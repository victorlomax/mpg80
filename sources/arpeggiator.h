#define _H_ARPEGGIATOR

// Arpeggiator statuses
#define STOPPED 0
#define STARTED 1
// Arpeggiator modes
#define UP      0
#define DOWN    1
#define UP&DOWN 2
#define RND     3

struct arpeggio
{
private:
  unsigned char _notes[16], // note pool
                _pos,       // current position
                _last;      // last note in the pool
  unsigned char _status;    // current status: 0000mmms, mmm=mode, s=status
public:                
  void add(unsigned char note),
       del(unsigned char note),
       play(unsigned int mode),
       stop(),
       reset();
  unsigned char next();
  bool          status();
};
