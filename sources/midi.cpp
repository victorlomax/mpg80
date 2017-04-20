#ifndef _H_MIDI
#include <midi.h>
#endif

class midi
{
	private:
		byte 	channel,
			events[MIDI_MAX];	
	public:
		byte	check_channel();
	...
}

midi	lower, upper;

uchar	voice[48];
PATCH	program[2];

byte	midi::check_channel(byte b)
	{ return (_omni_on)||(channel==b)?TRUE:FALSE; }

// This function decodes MIDI messages. 
// We recognized only some messages; all other messages are sent to both MIDI OUT and MIDI THRU, hence not decoded.
// MIDI messages are decoded byte per byte; the status is updated to keep track of what is we already received.
// REALTIME MESSAGES:
//      these messages can appear inside the MIDI flow; 
// STATUS
//  0x00: start
//  0xx1: 1 byte expected
//  0xx2: 2 bytes expected
//  0x3x: MTC quarter frame message, 1 data byte expected
//  0x4x: SONG pointer position message, 2 data bytes expected
//  0xfx: Start OF Exclusive, several data bytes expected
//  0x5x: Timing Clock message, 
//  0x8x: NOTE OFF message begining, 2 data bytes expected
//  0x9x: NOTE ON message begining, 2 data bytes expected
//  0xBx: CONTROL CHANGE message begining, 2 data bytes expected
//  0xCx: PROGRAM CHANGE message begining, 2 data bytes expected
//		Bank 0: MKS-80 Internal (0-63) and Cartridge (64-127)
//		Bank 1 - 127: MPG-80
//  
byte decode(byte b, byte status)
{
	if(b>0x7f)	// Control byte
	{
		switch(b<0xf0?b&0xf0:b)
		{
			case 0x80: 	if(!(lower.check_channel(b)&&arp_lower.started())
					||(upper.check_channel(b)&&arp_upper.started()))
					return NOF+2;	// Note off
			case 0x90: 	return NON+2;	// Note on
			case 0xa0:	midi.send(b);	// Byte copied to the output
					return PAT+2;	// Polyphonic Aftertouch
			case 0xb0:	return CCH+2;	// Control change
			case 0xc0:	if(midi.bank==0) midi.send(b);
					return PCH+1;	// Program change
			case 0xd0:	midi.send(b);	// Byte copied to the output
					return CHP+1;	// Monophonic aftertouch
			case 0xe0:	midi.send(b);	// Byte copied to the output
					return PBD+2;	// pitch bend
			case 0xf0:	return SOX;	// sysex
			case 0xf1:	return MTC+1;	// MTC Quarter Frame
			case 0xf2:	return SPP;	// Song pointer position
			case 0xf3:	return SMM+2;	// Song Select, 2 DATA bytes following
			case 0xf4:	return SMM;	// not defined
			case 0xf5:	return SMM;	// not defined
			case 0xf6:	midi.send(b);	// Byte copied to the output
					return SMM;	// Tune request (ignored)
			case 0xf7:	return SMM;	// EOX
			case 0xf8:	return CLK;	// (system real time) Timing clock (24 ppqn)
			case 0xf9:	return SMM;	// not defined
			case 0xfa: 	midi.send(b);	// Byte copied to the output
					arp_lower.start();
					arp_upper.start();
					return SMM;	// (system real time) Start
			case 0xfb:	midi.send(b);	// Byte copied to the output
					arp_lower.continue();
					arp_upper.continue();
					return SMM;	// (system real time) Continue
			case 0xfc:	midi.send(b);	// Byte copied to the output
					arp_lower.stop();
					arp_upper.stop();
					return SMM;		// (system real time) Stop
			case 0xfd:	return SMM;   		// not defined
			case 0xfe:	return SMM;		// Active sensing
			case 0xff:	midi.send(b);		// Byte copied to the output
					arp_lower.stop();	// stop arpeggiator
					arp_lower.del();	// and cleanup notes
					arp_upper.stop();
					arp_upper.del();
					return SMM;	// (system real time) Reset
		}
	}
	else
	{
		switch(status&0xf0)
		{
			case SMM: if(status&0x0f>0) status--; 
					midi.send(b);	// Byte copied to the output
					break;
			case MTC: // MTC quarter frame message, 1 data byte expected
			case SPP: // SONG pointer position message, 2 data bytes expected
			case SOX: // Start OF Exclusive, several data bytes expected
			case CLK: // Timing Clock message
			case NOF: // NOTE OFF message begining, 2 data bytes expected
			case NON: // NOTE ON message begining, 2 data bytes expected
			case CCH: // CONTROL CHANGE message begining, 2 data bytes expected
			case PCH: // PROGRAM CHANGE message begining, 2 data bytes expected
    		}
 }    switch(status & 0x0f)    {        case 1:        case 2:    }    }

