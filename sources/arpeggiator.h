#define _H_ARPEGGIATOR

// Arpeggiator statuses
#define ARP_STOPPED 0
#define ARP_STARTED 4

// Arpeggiator modes
#define ARP_UP      16
#define ARP_UPDOWN  20
#define ARP_DOWN    8
#define ARP_DOWNUP  52
#define ARP_RND     32

#define ARP_HOLD  64

#define ARP_MAX 16

class arpeggiator
{
  byte _notes[ARP_MAX], // note pool
       _octaves,   // Octaves
       _tempo,     // Tempo
       _pos,       // current position (0 to 15)
       _status;    // current status: [0hmm msoo]
                    // oo = octaves (0-3 = 1 to 4 octaves)
                    // s = stopped (bit=0) or started (bit=1)
                    // mmm = mode. 001=down, 010=up, 011=up&down, 1xx=random
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
