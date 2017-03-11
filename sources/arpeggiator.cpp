#ifndef _H_ARPEGGIATOR
#include "arpeggiator.h"
#endif

// Arpeggiator range:
// In bpm: 40 - 240 bpm
// In mHz: 666.67 - 4000
// In µS : 62500 - 10417 @ 24 ppqn

// Constructor
void arpeggiator::arpeggiator()
{
    for (int i=0; i < ARP_MAX; i++)
        channel[0]._status & ARP_HOLD)
        channel[midi_channel]._status &= !ARP_HOLD;
    else
        channel[midi_channel]._status |= ARP_HOLD;
}

// Hold the arpeggio
void arpeggiator::hold(bool midi_channel, byte mode)
{
    if (channel[midi_channel]._status & ARP_HOLD)
        channel[midi_channel]._status &= !ARP_HOLD;
    else
        channel[midi_channel]._status |= ARP_HOLD;
}

// Add a note to the arpeggio
void arpeggiator::add(byte midi_channel, byte note)
{
    // If it's in hold mode and you are not holding any notes down,
    // it continues to play the previous arpeggio. Once you press
    // a new note, it resets the arpeggio and starts a new one.
    if (notesHeld==0 && channel[midi_channel]._status & ARP_HOLD) 
      resetNotes();

    notesHeld++;


  // find the right place to insert the note in the notes array
  for (int i=0; i < channel[midi_channel]._last; i++)
  {
        if (channel[midi_channel]._notes[i] == note) return;   // already in arpeggio
        if (channel[midi_channel]._notes[i] < note) continue;  // ignore the notes below it
        // once we reach the first note in the arpeggio that's higher
        // than the new one, scoot the rest of the arpeggio array over 
        // to the right
        for (int j = channel[midi_channel]._last; j > i; j--)
            channel[midi_channel]._notes[j] = channel[midi_channel]._notes[j-1];

        // and insert the note
        channel[midi_channel]._notes[i] = note;
        return;        
    }
}

// Remove a note from the arpeggio
void arpeggiator::del(byte midi_channel, byte note)
{
        if (velocity == 0) { // note released
      if (!hold && notes[i] >= pitch) { 

        // shift all notes in the array beyond or equal to the
        // note in question, thereby removing it and keeping
        // the array compact.
        if (i < sizeof(notes))
          notes[i] = notes[i+1];
      }

}

// Remove ALL notes from the arpeggio
void arpeggiator::del(byte midi_channel)
{
    channel[midi_channel]._last=0;
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

arpeggiator lower_arp, upper_arp;
