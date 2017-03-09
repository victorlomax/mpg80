#ifndef _H_ARPEGGIATOR
#include "arpeggiator.h"
#endif

// Arpeggiator range:
// In bpm: 40 - 240 bpm
// In mHz: 666.67 - 4000
// In µS : 62500 - 10417 @ 24 ppqn

// Hold the arpeggio
void arpeggiator::hold(byte channel, byte mode)
{
}

// Add a note to the arpeggio
void arpeggiator::add(byte channel, byte note)
{
}

// Remove a note from the arpeggio
void arpeggiator::del(byte channel, byte note)
{
}

// Remove ALL notes from the arpeggio
void arpeggiator::del(byte channel)
{
}

// Play the arpeggio
void arpeggiator::play(byte channel, word mode, word octaves, word)
{
    _status |= 0x01;
}

// Stop the arpeggiator
void arpeggiator::stop()
{
    _status &= 0xfe;
    _pos = 0;
}

// Reset the arpeggio to the first note
void arpeggiator::reset()
{
}

// Reset the arpeggio to the first note
void arpeggiator::reset()
{
}


struct arpeggiator
{
private:
  unsigned char _notes[16], // note pool
                _pos,       // current position
                _last;      // last note in the pool
  unsigned char _status;    // current status: 0000mmms, mmm=mode, s=status
       reset();
  unsigned char next();
  bool          status();
};
