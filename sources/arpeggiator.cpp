#ifndef _H_ARPEGGIATOR
#include "arpeggiator.h"
#endif

// Arpeggiator range:
// In bpm: 40 - 240 bpm
// In mHz: 666.67 - 4000
// In µS : 62500 - 10417 @ 24 ppqn

// Hold the arpeggio
void arpeggiator::hold(byte midi_channel, byte mode)
{
    if (channel[midi_channel&0x01]._status && HOLDON)
        channel[midi_channel&0x01]._status &= !HOLDON;
    else
        channel[midi_channel&0x01]._status |= HOLDON;
}

// Add a note to the arpeggio
void arpeggiator::add(byte midi_channel, byte note)
{
}

// Remove a note from the arpeggio
void arpeggiator::del(byte midi_channel, byte note)
{
}

// Remove ALL notes from the arpeggio
void arpeggiator::del(byte midi_channel)
{
}

// Play the arpeggio
void arpeggiator::play(byte midi_channel, word mode, word octaves, word)
{
    _status |= 0x01;
}

// Stop the arpeggiator
void arpeggiator::stop(byte midi_channel)
{
    _status &= 0xfe;
    _pos = 0;
}
