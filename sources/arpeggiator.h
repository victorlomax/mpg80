#define _H_ARPEGGIATOR

// Arpeggiator statuses
#define STOPPED 0
#define STARTED 4

// Arpeggiator modes
#define UP      16
#define UPDOWN  20
#define DOWN    8
#define RND     32

typedef struct {
  byte _notes[16], // note pool: 16 notes max
       _octaves,   // Octaves
       _tempo,     // Tempo
       _pos,       // current position
       _last,      // last note of the pool
       _status;    // current status: [00mm msoo]
                    // oo = octaves (0-3 = 1 to 4 octaves)
                    // s = stopped (bit=0) or started (bit=1)
                    // mmm = mode. 001=down, 010=up, 011=up&down, 1xx=random
  } CHANNEL;

struct arpeggiator
{
private:
  CHANNEL channel[2];
public:                
  void add(byte),   // Add a new note to the pool
       del(byte),   // Remove a note from the pool
       del(),       // Remove ALL notes from the pool
       play(byte, byte, byte), // Start the arpeggiator
       stop();
  byte play();      // Play the next note
  bool status();
};
