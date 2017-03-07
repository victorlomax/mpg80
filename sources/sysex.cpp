/*
Parameter			Value range
0x00	LFO-1 RATE		0x00 - 0x64
0x01	LFO-1 DELAY TIME	0x00 - 0x64
0x02	LFO-1 WAVEFORM		0x00 = Random
				0x01 = Square Wave
				0x02 = Sawtooth Wave
				0x02 = Triangle Wave
0x03	VCO MOD LFO-1 DEPTH	0x00 - 0x64
0x04	VCO MOD ENV-1 DEPTH	0x00 - 0x64
0x05	PW			0x00 - 0x64
0x06	PWM			0x00 - 0x64
0x07	PWM MODE SEL		0x00 = Keyboard	
				0x01 = LFO-1	
				0x02 = ENV-1	
0x08	PWM POL			0x00 = Inverted
				0x01 = Normal
0x09	VCO KEY FOLLOW		0x00 - 0x64
0x0a	VCO SEL			0x00 = VCO-2	
				0x01 = OFF	
				0x02 = VCO-1	
0x0b	XMOD MANUAL DEPTH	0x00 - 0x64
0x0c	XMOD ENV-l DEPTH	0x00 - 0x64
0x0d	XMOD POL		0x00 = Inverted
				0x01 = Normal
0x0e	VCO-1 MOD		0x00 = Inverted
				0x01 = OFF	
				0x02 = Normal
0x0f	VCO-1 RANGE		0x24 - 0x54 (key 36 to 84, 60 = middle C at 8')
0x10	VCO-1 WAVEFORM		0x00 = Square Wave
				0x01 = Pulse Wave
				0x02 = Sawtooth Wave
				0x03 = Triangle Wave
0x11	VCO SYNC		0x00 = VCO-1 <- VCO-2
				0x01 = OFF
				0x02 = VCO-1 -> VCO-2
0x12	VCO-2 MOD		0x00 = Inverted
				0x01 = OFF
				0x02 = Normal
0x13	VCO-2 RANGE		0x00 = Low Frequency
				0x24 - 0x54 (key 36 to 84, 60 = middle C at 8')
				0x64 = High Frequency
0x14	VCO-2 FINE TUNE		0x00 - 0x64
0x15	VCO-2 WAVEFORM		0x00 = Noise
				0x01 = Pulse Wave
				0x02 = Sawtooth Wave
				0x03 = Triangle Wave
0x16	MIXER			0x00 - 0x64
0x17	HPF CUTOFF FREQ		0x00 - 0x64
0x18	VCF CUTOFF FREQ		0x00 - 0x64
0x19	VCF RESONANCE		0x00 - 0x64
0x1a	VCF ENV SEL		0x00 = ENV-2
				0x01 = ENV-1
0x1b	VCF ENV POL		0x00 = Inverted
				0x01 = Normal
0x1c	VCF MOD ENV DEPTH	0x00 - 0x64
0x1d	VCF MOD LFO-1 DEPTH	0x00 - 0x64
0x1e	VCF KEY FOLLOW		0x00 - 0x64
0x1f	VCA ENV-2 LEVEL		0x00 - 0x64
0x20	VCA MOD LFO-1 DEPTH	0x00 - 0x64
0x21	DYNAMICS TIME		0x00 - 0x64
0x22	DYNAMICS LEVEL		0x00 - 0x64
0x23	ENV RESET		0x00 = OFF
				0x01 = ON
0x24	ENV-1 DYNAMICS		0x00 = OFF
				0x01 = ON
0x25	ENV-1 ATTACK TIME	0x00 - 0x64
0x26	ENV-1 DECAY TIME	0x00 - 0x64
0x27	ENV-1 SUSTAIN LEVEL	0x00 - 0x64
0x28	ENV-1 RELEASE TIME	0x00 - 0x64
0x29	ENV-1 KEY FOLLOW	0x00 - 0x64
0x2a	ENV-2 DYNAMICS		0x00 = OFF
				0x01 =	ON
0x2b	ENV-2 ATTACK TIME	0x00 - 0x64
0x2c	ENV-2 DECAY TIME	0x00 - 0x64
0x2d	ENV-2 SUSTAIN LEVEL	0x00 - 0x64
0x2e	ENV-2 RELEASE TIME	0x00 - 0x64
0x2f	ENV-2 KEY FOLLOW	0x00 - 0x64
*/

/*
0x00	KEY MODE SELECT		0x00 = Dual
				0x0l = Split-1
				0x02 = Split-2
				0x03 = Whole
0x01	SPLIT POINT		0x15 - 0x6C (Note number 21 - 108)
0x02	BALANCE			0x00 - 0x64
0x03	TONE NUMBER		0x00 - 0x3F
0x04	OCT SHIFT		0x00 = 2 OCT Down
				0x01 = 1 OCT Down
				0x02 = Normal
				0x03 = 1 OCT Up
				0x04 = 2 OCT Up
0x05	ASSIGN MODE SELECT	0x00 = Solo
				0x01 = Unison-1
				0x02 = Unison-2
				0x03 = Poly-l
				0x04 = Poly-2
0x06	UNISON DETUNE		0x00 - 0x64
0x07	HOLD			0x00 = OFF
				0x01 = ON (always) (MIDI Damper messages ignored)
				0x02 = MIDI Damper messages recognized
0x08	GLIDE			0x00 - 0x64
0x09	BENDER SENS		0x00 - 0x64
0x0a	VCO-1 BEND		0x00 = OFF
				0x01 = Normal (Slightly more than 1 octave)
				0x02 = Wide (2.5 octaves)
0x0b	VCO-2 BEND		0x00 = OFF
				0x01 = Normal (Slightly more than 1 octave)
				0x02 = Wide (2.5 octaves)
0x0c	AFTER TOUCH SENS	0x00 - 0x64
0x0d	AFTER TOUCH MODE SELECT	0x00 = VCF Frequency
				0x01 = VCO LFO-2 MOD (1 and 2)
0x0e	LFO-2 RATE		0x00 - 0x64
*/

unsigned char	tone[96];
unsigned char	patch[30];
char*	values;

S00:	if(*(byte++)!=0xf0) goto S99;	// Start of Exclusive
	if(*(byte++)!=0x41) goto S99;	// Roland ID
	if(*(byte++)!=0x36) goto S99;	// Individual Parameter
	if((*byte!=midi_channel[LOWER])&&(*byte!=midi_channel[UPPER])) goto S99;	// Unit ID
	byte++;
	if(*byte!=0x20) goto S_TONE;	// Level #1: TONE PARAMETER
	if(*byte!=0x30) goto S_PATCH;	// Level #2: PATCH PARAMETER
	goto S99;
S_TONE:	display("LOADING TONE..");
	byte++;
	if((*byte&&0xfa)!=0x00) goto S99;
	values=tone+(*byte==0x01)?0:48;
	while(*byte!=0xfe)
	{
		i=*(byte++);		// Address
		*(values+i)=*(byte++);	// Value
	}
	goto S00;
S_PATCH:	display("LOADING PATCH..");
	byte++;
	if((*byte&&0xfa)!=0x00) goto S99;
	values=patch+(*byte==0x01)?0:15;
	while(*byte!=0xfe)
	{
		i=*(byte++);		// Address
		*(values+i)=*(byte++);	// Value
	}
	goto S00;
S99:	display("MIDI DATA ERROR!");
	goto S00;