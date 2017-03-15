#define _H_ARPEGGIATOR

// Arpeggiator statuses
#define ARP_STOPPED 0
#define ARP_STARTED 0x80

// Arpeggiator modes
#define ARP_UP      0x20
#define ARP_UPDOWN  0x30
#define ARP_DOWN    0x10
#define ARP_DOWNUP  0x50
#define ARP_RND     0x40
#define ARP_BOUNCE  0x60
#define ARP_PLAYED  0x70
#define ARP_HOLD    0x08

#define ARP_MAX 16

class arpeggiator
{
  byte _notes[ARP_MAX], // note pool
       _octaves,   // Octaves
       _tempo,     // Tempo
       _step,      // Sub step (0->255)
       _pos,       // current position (nnnnxxoo: nnnn = current position in _notes[], oo=current octave)
       _mode;      // current mode: [smmmhxoo]
                    // oo = octaves (0-3 = 1 to 4 octaves)
                    // s = stopped (bit=0) or started (bit=1)
                    // mmm = mode. 001=down, 010=up, 011=up&down, 101=down&up, 100=random, 110=bounce, 111=played
                    // h = hold
public:                
  void add(byte),   // Add a new note to the pool
       del(byte),   // Remove a note from the pool
       del(),       // Remove ALL notes from the pool
       hold(byte),       // Hold ON or OFF
       play(byte, byte), // Start the arpeggiator
       stop();
  byte play();      // Play the next note
  bool status();
};
