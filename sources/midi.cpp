
#ifndef _H_MIDI
#include <midi.h>
#endif

byte	midi_channel[2];
char events[MIDI_MAX];
uchar	voice[48];
PATCH	program[2];

// This function decodes MIDI messages. 
// We recognized only some messages; all other messages are sent to both MIDI OUT and MIDI THRU, hence not decoded.
// MIDI messages are decoded byte per byte; the status is updated to keep track of what is we already received.
// REALTIME MESSAGES:
//      these messages can appear inside the MIDI flow; 
// STATUS
//  0x00: start
//  0xx1: discard next byte
//  0xx2: discard next 2 bytes
//  0x3x: MTC quarter frame message, 1 data byte expected
//  0x4x: SONG pointer position message, 2 data bytes expected
//  0xfx: Start OF Exclusive, several data bytes expected
//  0x5x: Timing Clock message, 
//  0x8x: NOTE OFF message begining, 2 data bytes expected
//  0x9x: NOTE ON message begining, 2 data bytes expected
//  0xBx: CONTROL CHANGE message begining, 2 data bytes expected
//  0xCx: PROGRAM CHANGE message begining, 2 data bytes expected
//  
byte decode(byte b, byte status)
{
    // These command bytes are ignored; we don't change the status
    switch(b)
    {
      case 0xf3:  // Song Select, 2 DATA bytes following
      case 0xf4:  // not defined
      case 0xf5:  // not defined
      case 0xf6:  // Tune request
      case 0xf9:  // not defined
      case 0xfd:  // not defined
      case 0xfe:  // Active sensing 
                return status;  // Discarded
    }
    switch(b & 0xf0)
    {
      case 0xa0:  // Polyphonic Aftertouch
      case oxd0:  // Monophonic aftertouch   
      case 0xe0: return status; // pitch bend
    }
    // Thse messages are recognized; we return the new status
    switch(b)
    {
      case 0xf0: return 0xf0; // sysex
      case 0xf1: return 0x30; // MTC Quarter Frame
      case 0xf2: return 0x40; // Song pointer position
      case 0xf7:  // EOX
      case 0xf8:  // (system real time) Timing clock (24 ppqn)
      case 0xfa:  // (system real time) Start
      case 0xfb:  // (system real time) Continue
      case 0xfc:  // (system real time) Stop
      case 0xff: return b; // (system real time) Reset
    }
    switch(b & 0xf0)    
    {
      case 0x80:    // Note OFF
      case 0x90:    // Note ON
      case 0xb0:    // Control change
      case 0xc0:    // Program change
              if((b & 0x0f == midi_channel[0])||(b & 0x0f == midi_channel[1]))
                return b & 0xf0;
    }
