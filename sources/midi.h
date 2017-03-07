#define _H_MIDI
typedef unsigned char uchar;

/* ******************************
  The TONE structure
******************************

Parameter			Value range
0x00	LFO-1 RATE		0x00 - 0x64
0x01	LFO-1 DELAY TIME	0x00 - 0x64
0x02	LFO-1 WAVEFORM		0x00 = Random
				0x01 = Square Wave
				0x02 = Sawtooth Wave
				0x02 = Triangle Wave
0x03	VCO MOD LFO-1 DEPTH	0x00 - 0x64
0x04	VCO MOD ENV-1 DEPTH	0x00 - 0x64
0x05	PW			0x00 - 0x64
0x06	PWM			0x00 - 0x64
0x07	PWM MODE SEL		0x00 = Keyboard	
				0x01 = LFO-1	
				0x02 = ENV-1	
0x08	PWM POL			0x00 = Inverted
				0x01 = Normal
0x09	VCO KEY FOLLOW		0x00 - 0x64
0x0a	VCO SEL			0x00 = VCO-2	
				0x01 = OFF	
				0x02 = VCO-1	
0x0b	XMOD MANUAL DEPTH	0x00 - 0x64
0x0c	XMOD ENV-l DEPTH	0x00 - 0x64
0x0d	XMOD POL		0x00 = Inverted
				0x01 = Normal
0x0e	VCO-1 MOD		0x00 = Inverted
				0x01 = OFF	
				0x02 = Normal
0x0f	VCO-1 RANGE		0x24 - 0x54 (key 36 to 84, 60 = middle C at 8')
0x10	VCO-1 WAVEFORM		0x00 = Square Wave
				0x01 = Pulse Wave
				0x02 = Sawtooth Wave
				0x03 = Triangle Wave
0x11	VCO SYNC		0x00 = VCO-1 <- VCO-2
				0x01 = OFF
				0x02 = VCO-1 -> VCO-2
0x12	VCO-2 MOD		0x00 = Inverted
				0x01 = OFF
				0x02 = Normal
0x13	VCO-2 RANGE		0x00 = Low Frequency
				0x24 - 0x54 (key 36 to 84, 60 = middle C at 8')
				0x64 = High Frequency
0x14	VCO-2 FINE TUNE		0x00 - 0x64
0x15	VCO-2 WAVEFORM		0x00 = Noise
				0x01 = Pulse Wave
				0x02 = Sawtooth Wave
				0x03 = Triangle Wave
0x16	MIXER			0x00 - 0x64
0x17	HPF CUTOFF FREQ		0x00 - 0x64
0x18	VCF CUTOFF FREQ		0x00 - 0x64
0x19	VCF RESONANCE		0x00 - 0x64
0x1a	VCF ENV SEL		0x00 = ENV-2
				0x01 = ENV-1
0x1b	VCF ENV POL		0x00 = Inverted
				0x01 = Normal
0x1c	VCF MOD ENV DEPTH	0x00 - 0x64
0x1d	VCF MOD LFO-1 DEPTH	0x00 - 0x64
0x1e	VCF KEY FOLLOW		0x00 - 0x64
0x1f	VCA ENV-2 LEVEL		0x00 - 0x64
0x20	VCA MOD LFO-1 DEPTH	0x00 - 0x64
0x21	DYNAMICS TIME		0x00 - 0x64
0x22	DYNAMICS LEVEL		0x00 - 0x64
0x23	ENV RESET		0x00 = OFF
				0x01 = ON
0x24	ENV-1 DYNAMICS		0x00 = OFF
				0x01 = ON
0x25	ENV-1 ATTACK TIME	0x00 - 0x64
0x26	ENV-1 DECAY TIME	0x00 - 0x64
0x27	ENV-1 SUSTAIN LEVEL	0x00 - 0x64
0x28	ENV-1 RELEASE TIME	0x00 - 0x64
0x29	ENV-1 KEY FOLLOW	0x00 - 0x64
0x2a	ENV-2 DYNAMICS		0x00 = OFF
				0x01 =	ON
0x2b	ENV-2 ATTACK TIME	0x00 - 0x64
0x2c	ENV-2 DECAY TIME	0x00 - 0x64
0x2d	ENV-2 SUSTAIN LEVEL	0x00 - 0x64
0x2e	ENV-2 RELEASE TIME	0x00 - 0x64
0x2f	ENV-2 KEY FOLLOW	0x00 - 0x64
*/

typedef union {
    uchar	dump[48],
    struct {
      struct {
           uchar	rate,
				          delay,
				          waveform;
		  } lfo-1;
		  struct {
			    uchar	lfo-1,
				        env-1;
		  } vco-mod;
		  struct {
			    uchar	pw,
				        pmw,
				        mod,
				        pol;
		  } pwm;
		  struct {
			    uchar	key-follow,
				        sel;
		  } vco;
		  struct {
			    uchar	manual-depth,
				        env-1-depth
				        pol;
		  } xmod;
		  struct {
			    uchar	mod,
				        range,
				        waveform;
		  } vco-1;
		  uchar	vco-sync;
		  struct {
			    uchar	mod,
        				range,
        				tune,
        				waveform;
		  } vco-2;
		  uchar	mixer;
		  struct {
    			uchar	cutoff;
  		} hpf;
  		struct {
    			uchar	cutoff,
        				resonance,
        				env-del,
        				env-pol,
        				env-depth,
        				lfo-depth,
        				key-follow;
		  } vcf;
		  struct {
    			uchar	env-level,
        				lfo-depth;
		  } vca;
		  struct {
    			uchar	time,
        				level;
		  } dynamics;
		  uchar	env-reset;
		  struct {
			    uchar	dynamics,
				        attack,
				        decay,
				        sustain,
				        release,
				        key-follow;
		  }[2] env;
  } tone;
} TONE;

/* ******************************
    The PATCH structure
******************************

0x00	KEY MODE SELECT		0x00 = Dual
				0x0l = Split-1
				0x02 = Split-2
				0x03 = Whole
0x01	SPLIT POINT		0x15 - 0x6C (Note number 21 - 108)
0x02	BALANCE			0x00 - 0x64
0x03	TONE NUMBER		0x00 - 0x3F
0x04	OCT SHIFT		0x00 = 2 OCT Down
				0x01 = 1 OCT Down
				0x02 = Normal
				0x03 = 1 OCT Up
				0x04 = 2 OCT Up
0x05	ASSIGN MODE SELECT	0x00 = Solo
				0x01 = Unison-1
				0x02 = Unison-2
				0x03 = Poly-l
				0x04 = Poly-2
0x06	UNISON DETUNE		0x00 - 0x64
0x07	HOLD			0x00 = OFF
				0x01 = ON (always) (MIDI Damper messages ignored)
				0x02 = MIDI Damper messages recognized
0x08	GLIDE			0x00 - 0x64
0x09	BENDER SENS		0x00 - 0x64
0x0a	VCO-1 BEND		0x00 = OFF
				0x01 = Normal (Slightly more than 1 octave)
				0x02 = Wide (2.5 octaves)
0x0b	VCO-2 BEND		0x00 = OFF
				0x01 = Normal (Slightly more than 1 octave)
				0x02 = Wide (2.5 octaves)
0x0c	AFTER TOUCH SENS	0x00 - 0x64
0x0d	AFTER TOUCH MODE SELECT	0x00 = VCF Frequency
				0x01 = VCO LFO-2 MOD (1 and 2)
0x0e	LFO-2 RATE		0x00 - 0x64
*/
typedef union {
	  uchar	dump[48],
	  struct {
		    uchar	key-mode-select,
			        split-point,
			        balance,
			        tone,
			        octave,
			        assign-mode,
			        unison-detune,
			        hold,
			        glide,
			        bender-sens,
			        vco-bend[2];
		  struct {
    			uchar	sens,
        				mode;
		  } after-touch;
		  uchar	lfo2-rate;
	  } patch;
} PATCH;

/* ******************************
    The ARPEGGIO structure
******************************

Parameter			Value range
0x00	ARPEGGIO MODE		0x00 = Off
				0x01 = Up
				0x02 = Down
				0x03 = U&D (Up & Down)
0x01	ARPEGGIO RATE		0x00 - 0x64
0x02	ARPEGGIO SYNC		0x00 = Off (Internal clock)
				0x01 = Ext (External clock)
				0x02 = MIDI (MIDI Clock)
				0x02 = MTC (MIDI Time Code)
0x03	ARPEGGIO OCTAVE		0x00 - 0x03
*/

typedef struct {
    uchar	mode,
	      	rate,
		      sync,
		      octave;
} ARPEGGIO;

#define	UPPER	0
#define LOWER	1

char	midi_channel[2];
uchar	voice[48];
PATCH	program[2];

