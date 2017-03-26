#ifndef _H_ARPEGGIATOR
#include "arpeggiator.h"
#endif

#ifndef _H_TIMERONE
#include "TimerOne.h"
#endif

// Arpeggiator range:
// In bpm: 40 - 240 bpm
// In mHz: 666.67 - 4000
// In µS : 62500 - 10417 @ 24 ppqn

void callback()
{
      digitalWrite(10, digitalRead(10) ^ 1);
}

// Constructor
void arpeggiator::arpeggiator()
{
    for (int i=0; i < ARP_MAX; i++)
    {
        _index[i]=0x00;
        _notes[i]=0xff;
    }
    _pos=0;
    _status=0;
}

// Hold the arpeggio
void arpeggiator::hold(byte mode)
{
    if (mode==0)
        _status &= !ARP_HOLD;
    else
        _status |= ARP_HOLD;
}

// Add a note to the arpeggio
void arpeggiator::add(byte note)
{
    // If it's in hold mode and you are not holding any notes down,
    // it continues to play the previous arpeggio. Once you press
    // a new note, it resets the arpeggio and starts a new one.
    if (notesHeld==0 && _status & ARP_HOLD) 
      resetNotes();

    notesHeld++;
      
    if (_notes[_index[ARP_MAX]]!=0xff) return; // Full!

  // find the right place to insert the note in the notes array
  for (int i=0; i < ARP_MAX && notes[i][ARP_NOTES]!=0xff; i++)
  {
        if (_notes[i][_notes[i][ARP_INDEX]] == note) return;   // already in arpeggio
        if (_notes[i][0] < note) continue;  // ignore the notes below it
        // once we reach the first note in the arpeggio that's higher
        // than the new one, scoot the rest of the arpeggio array over 
        // to the right
        for (int j = ARP_MAX; j > i; j--)
            _notes[j] = _notes[j-1];

        // and insert the note
        _notes[i] = note;
        return;        
    }
}

// Remove a note from the arpeggio
void arpeggiator::del(byte note)
{
    byte found=FALSE;
    // Seek for the note
    for (int i=0; i < ARP_MAX && notes[i]!=0xff; i++)
    {
        if (_notes[i] == note) 
            { found=TRUE; continue; }
        if (found == TRUE)
            _notes[i-1] = _notes[i];
    }
    _notes[15]=0xff;
}

// Remove ALL notes from the arpeggio
void arpeggiator::del()
{    
    for (int i=0; i < ARP_MAX; i++)
        _notes[i]=0xff;
    _pos=0;
}

// Play the arpeggio
void arpeggiator::play(byte mode, byte octaves)
{
    _status |= ARP_STARTED;
    Timer1.initialize(500000);
    Timer1.pwm(9, 512);
    Timer1.attachInterrupt(callback);
    switch(mode)
    {
        case ARP_UP: break;
        case ARP_UPDOWN: break;
        case ARP_DOWN: break;
        case ARP_DOWNUP: break;
        case ARP_BOUNCE: break;
        case ARP_RND: break;
    }
}

// Play the next note in the arpeggio
byte arpeggiator::play()
{
    if(_status && 0x01)
        switch(mode)
        {
            case ARP_UP: if(_notes[_pos]==0xff) {_pos=0; if((_status & 0x03)==_octaves) _status &= 0xfc; break;
            case ARP_UPDOWN: break;
            case ARP_DOWN: break;
            case ARP_DOWNUP: break;
            case ARP_RND: break;
        }
}

// Stop the arpeggiator
void arpeggiator::stop(byte midi_channel)
{
    _status &= 0xfe;
    _pos = 0;
}

arpeggiator arp_lower, arp_upper;
