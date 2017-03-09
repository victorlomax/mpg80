#define _H_ARPEGGIATOR

// Arpeggiator statuses
#define STOPPED 0
#define STARTED 4

// Arpeggiator modes
#define UP      16
#define UPDOWN  20
#define DOWN    8
#define RND     32

struct arpeggiator
{
private:
  byte _notes[16], // note pool: 16 notes max
       _octaves,   // Octaves
       _tempo,     // Tempo
       _pos,       // current position
       _last,      // last note in the pool
       _status;    // current status: [00mm msoo]
                    // oo = octaves (0-3 = 1 to 4 octaves)
                    // s = stopped (bit=0) or started (bit=1)
                    // mmm = mode. 001=down, 010=up, 011=up&down, 1xx=random
public:                
  void add(byte note),
       del(byte note),
       play(byte mode, byte octaves, byte tempo), // Start the arpeggiator
       stop(),
       reset();
  unsigned char play(void);     // Play the next note
  bool          status();
};
