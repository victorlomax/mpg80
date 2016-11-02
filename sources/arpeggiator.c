#include "midi_arpeggiator.h"

// MIDI housekeeping
char status = 0;
char channel = 0;
char byteNumber = 1;
char byteOne = 0;
// event list (fixed size)
Event events[8];
// ring buffer
char fifo[BUFFER_SIZE];
char fifoInIndex = 0;
char fifoOutIndex = 0;
// arpeggiator stuff
char currentPattern = 1; // this will trigger loading for the first time.
char newPattern = 0;
char currentNote = 0;
char newNote = 0;
char currentVelocity = 64;
char newVelocity = 64;
//
char play = 0;
char beatCounter = 0; // internal beat, which is 1/16th of the actual beat (bpm)
unsigned int bpm = 120;
unsigned int newCCR = 0;

void main(void) {

	WDTCTL = WDTPW + WDTHOLD; // disable WDT
	BCSCTL1 = CALBC1_16MHZ; // 16MHz clock
	DCOCTL = CALDCO_16MHZ;
	// bpm/measure LEDs
	P1OUT &= ~(LED1_PIN + LED2_PIN);
	P1DIR |= LED1_PIN + LED2_PIN;
	// setup USCIA
	P1SEL |= MIDI_IN_PIN + MIDI_OUT_PIN;
	P1SEL2 |= MIDI_IN_PIN + MIDI_OUT_PIN;
	UCA0CTL1 |= UCSSEL_2;
	UCA0BR0 = 0x00; // 31.25
	UCA0BR1 = 0x02; //
	UCA0CTL1 &= ~UCSWRST;
	IE2 |= UCA0RXIE + UCA0TXIE;
	// setup Timer
	TACTL = TASSEL_2 + MC_1 + ID_3; // SMCLK/8, up mode
	newCCR = getCCR(bpm);
	CCR0 = newCCR;

	_bis_SR_register(GIE);

}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0(void) {
	CCR0 = newCCR; // smoother speed transition
	char c = 0; // general use counter
	if (beatCounter == 64) { // end of measure, go to the beginning
		beatCounter = 0;
		// for some reason, my sound modules do not respond properly to all notes off
		if (play == 0) { // we will finish current pattern before stopping
			CCTL0 &= ~CCIE;
			return;
		}
	}
	// handle changes
	if (beatCounter == 0 // at the beginning of the measure...
			&& (newNote != currentNote || newPattern != currentPattern
					|| newVelocity != currentVelocity)) { // ...update events if anything has changed
		while (c < 4) { // 8 events, 4 pairs of notes, one on and one off
			char note = getNextNaturalNoteAdd( // if you don't use getNextNaturalNote in handleNoteOn, use getNextNaturalNote here
					newNote, patterns[newPattern][c].noteOffset);
			// on
			events[c].beat = patterns[newPattern][c].beat;
			events[c].note = note;
			events[c].onOff = 1;
			// off
			events[c + 4].beat = patterns[newPattern][c].beat
					+ patterns[newPattern][c].noteDuration; // assuming that the beat and the duration is correctly calculated, sum must be < 64
			events[c + 4].note = note;
			events[c + 4].onOff = 0;
			c++;
		}
		currentPattern = newPattern;
		currentNote = newNote;
		currentVelocity = newVelocity;
	}
	// now let's take care of the events
	c = 0;
	while (c < 8) { // go through all events and play when event's beat matches beatCounter
		if (events[c].beat == beatCounter) {
			playNote(events[c].note, events[c].onOff);
		}
		c++;
	}
	// let's blink LEDs
	if (beatCounter == 0)
		P1OUT |= LED1_PIN + LED2_PIN; // beginning of the measure
	else if ((beatCounter & 0x0F) == 0)
		P1OUT |= LED2_PIN; // beginning of each beat (bpm)
	else {
		P1OUT &= ~(LED2_PIN + LED1_PIN); // LEDs off
	}
	//
	beatCounter++;
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
	handleMIDI(UCA0RXBUF);
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void) {
	sendMIDI(); // done sending previous byte, see if there's anything else to send
}

void playNote(char noteNumber, char onOff) {
	push((onOff ? MIDI_NOTE_ON : MIDI_NOTE_OFF) + channel);
	push(noteNumber);
	push(currentVelocity);
	startMIDI();
}

void soundOff() {
	//push(MIDI_CONTROLLER + channel);
	//push(MIDI_ALL_SOUND_OFF);
	//push(0);
	push(MIDI_CONTROLLER + channel);
	push(MIDI_ALL_NOTES_OFF);
	push(0);
	startMIDI();
}

void handleNoteOn(char channel, char noteNumber, char velocity) {
	//newNote = noteNumber;
	newNote = getNextNaturalNote(noteNumber);
	newVelocity = velocity;
	if (play == 0) { // start if stopped
		beatCounter = 0;
		CCTL0 |= CCIE;
		play = 1;
	}
}

void push(char data) {
	fifo[++fifoInIndex & BUFFER_MASK] = data;
}

/* send MIDI should be used by TX ISR only */
void sendMIDI() {
	if (fifoInIndex != fifoOutIndex) { // send whatever is in the buffer
		while (!(IFG2 & UCA0TXIFG))
			;
		UCA0TXBUF = fifo[++fifoOutIndex & BUFFER_MASK];
	}
}
/* start MIDI should be called after queueing up all messages and associated data for those messages */
void startMIDI() {
	if (fifoInIndex != fifoOutIndex) { // send whatever is in the buffer
		IE2 &= ~UCA0TXIE; // disable TX interrupts, not needed if this is called from ISR only
		if (IFG2 & UCA0TXIFG)
			UCA0TXBUF = fifo[++fifoOutIndex & BUFFER_MASK];
		IE2 |= UCA0TXIE; // enable TX interrupts
	}
}

void handleProgramChange(char pcChannel, char number) {
	if (channel == pcChannel) {
		newPattern = number & 0x07; // only 8 preset patterns right now so each program number is masked
	}
}

void handleControlChange(char ccChannel, char controllerNumber, char value) {
	if (channel == ccChannel) {
		if (controllerNumber == 2) { // any controller can be used, this was convenient with MIDI Monitor and with XP-50
			bpm = 120 + (value * 3); // range is 120bpm to 501bpm
			newCCR = getCCR(bpm);
		}
	}
}

unsigned int getCCR(unsigned int bpm) {
	// calculate CCR based on clock and bpm value
	// 16MHz (SMCLK) / 8 (IDx divider in TACTL) = 2
	// 3750000 is a constant
	// 2 * 3750000 = 7500000
	// CCR time should equal 1/64th note (4/4)
	return 7500000 / bpm; // calculate CCR based on clock (16MHz / 8 (pre-scaler) = 2) and bpm value,
}

char getNextNaturalNote(char note) { // if the note is not a natural note, return next natural note
	char offset = note;
	while (offset > 11) {
		offset -= 12;
	}
	return note + noteOffset[offset];
}

char getNextNaturalNoteAdd(char naturalNote, char offset) {
	char n = naturalNote;
	while (n > 11) {
		n -= 12;
	}
	char o = offset;
	while (o > 6) {
		o -= 7;
	}
	return naturalNote + offset + noteLookup[noteToNatural[n]][o];
}

void handleMIDI(char rxByte) {
	if (rxByte & BIT7) { // is it status?
		if ((rxByte & MIDI_SYSTEM_REAL_TIME) == MIDI_SYSTEM_REAL_TIME) { // System Real Time
			switch (rxByte) { // having this switch here will reduce overhead
			case MIDI_START: // handleMIDIStart();
				if (play == 0) {
					play = 1;
					beatCounter = 0;
					CCTL0 |= CCIE;
				}
				break;
			case MIDI_STOP: // handleMIDIStop();
				if (play == 1) {
					play = 0;
					//CCTL0 &= ~CCIE;
					//soundOff(); // weird, notes keep playing, let's try something different
				}
				break;
			}
		} else if ((rxByte & MIDI_SYSTEM_COMMON) == MIDI_SYSTEM_COMMON) { // System Common category?
			status = rxByte;
			// this is not needed but I left it just in case it is
//			switch (status) { // unused
//			case MIDI_SYSEX_BEGIN: // start SysEx
//				break;
//			case MIDI_SYSEX_END: // end SysEx
//				break;
//			case MIDI_TUNE_REQUEST: // Tune Request
//				break;
//			}
			byteNumber = 1;
		} else { // Voice category
			channel = rxByte & MIDI_CHANNEL_MASK;
			status = rxByte & 0xF0;
			byteNumber = 1;
		}
	} else { // it's data byte
		if (byteNumber == 1) { // is it first data byte?
			switch (status) {
			case MIDI_PROGRAM_CHANGE: // Program Change
				handleProgramChange(channel, rxByte);
				break;
				// we are not using cases below, but we need them
			case MIDI_CHANNEL_PRESSURE: //handleChannelPressure(channel, rxByte);
				break;
			case MIDI_SYSEX: // add To SysEx Buffer
				break;
			case MIDI_QUARTER_FRAME_MSG: // MTC Quarter Frame Message
				break;
			case MIDI_SONG_SELECT: // Song Select
				break;
			default:
				byteNumber = 2; // second byte will follow
				byteOne = rxByte;
			}
		} else {
			byteNumber = 1; // next byte will be first byte
			switch (status) {
			case MIDI_NOTE_ON:
				if (byteOne) {
					handleNoteOn(channel, byteOne, rxByte);
					break;
				}
			case MIDI_CONTROLLER: // Control Change
				handleControlChange(channel, byteOne, rxByte);
				break;
				// this is not needed but I left it just in case it is
//			case MIDI_NOTE_OFF: //handleNoteOff(channel, byteOne, rxByte);
//				break;
//			case MIDI_AFTERTOUCH: //handleAfterTouch(channel, byteOne, rxByte);
//				break;
//			case MIDI_PITCH_WHEEL: //handlePitchWheel(channel, byteOne, rxByte);
//				break;
//			case MIDI_SONG_POSITION_PTR: // Song Position Pointer
//				break;
			}
		}
	}
}
