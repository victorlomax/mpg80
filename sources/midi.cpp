
#ifndef _H_MIDI
#include <midi.h>
#endif

char	midi_channel[2];
char events[MIDI_MAX];
uchar	voice[48];
PATCH	program[2];

byte decode(byte b, byte status)
{
    // These command bytes are ignored; we don't change the status
    switch(b)
    {
      case 0xf3:  // Song Select
      case 0xf4:  // not defined
      case 0xf5:  // not defined
      case 0xf6:  // Tune request
      case 0xf9:  // not defined
      case 0xfd:  // not defined
      case 0xfe:  // Active sensing 
                return status;
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
      case 0xf0:  // sysex
      case 0xf1:  // MTC Quarter Frame
      case 0xf2:  // Song pointer position
      case 0xf7:  // EOX
      case 0xf8:  // (system real time) Timing clock (24 ppqn)
      case 0xfa:  // (system real time) Start
      case 0xfb:  // (system real time) Continue
      case 0xfc:  // (system real time) Stop
      case 0xff: return b; // (system real time) Reset
    }
    switch(b & 0xf0)    
    {
      case 0x80: 
