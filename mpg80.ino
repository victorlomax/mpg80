// ATMega328

// Pin 28 (PC5) will be use for analog read
// Pin 23-26 (PC0-PC4) : select the 4067 channel
// Pin 14-16 (PB0-PB2) : select the board


// MASK: expected value
//      bits  
//   0:  -   no value (void)
//   1:  1   one value (touch switch with latch; +5V or 0V)
//   2:  1   two values (+5V or 0V)
//   3:  2   three values (+5V, 2.5V or 0V)
//   4:  2   four values (+5V, 3.3V, 1.7V, or 0V)
//   5:  3   five values (+5V, 3.7V, 2.5V, 1.3V or 0V)
//  16:  4   16 values
// 127:  7   continuous values

#define	MODIFIED	0x80

#define	_80PC	0x0332
#define	_75PC	0x02ff
#define	_66PC	0x02aa
#define	_60PC	0x0265
#define	_50PC	0x01ff
#define	_40PC	0x0198
#define	_33PC	0x0155
#define	_25PC	0x00ff
#define	_20PC	0x00cb

int	analogPin = 5;

void scan(char *mask, char *values)
{
	int board, channel, value;

	pinMode(analogPin, INPUT);
	analogReference(DEFAULT);

	for (board=0; board<6; board++)
	{
		digitalwrite(PORTC, board & 0x07);
		for (channel=0; channel<16; channel++, values++, mask++)
		{
			digitalwrite(PORTB, channel & 0x0f);
			value=analogRead(analogPin);
			switch(*mask)
			{
				case 0: break;
				case 1: 
				case 2: value=(value>_50PC)?0x0001:0x0000; break;
				case 3: value=(value>_66PC)?0x0002:((value>_33PC)?0x0001:0x0000); break;
				case 4: if(value<_25PC) value=0x0000;
					if(value>_75PC) value=0x0003;
					if(value>_50PC) value=0x0002;
					if(value>0x0003) value=0x0001;
					break;
				case 5: if(value<_20PC) value=0x0000;
					if(value>_80PC) value=0x0004;
					if(value>_60PC) value=0x0003;
					if(value>_40PC) value=0x0002;
					if(value>0x0004) value=0x0001;
					break;
				case 16: analogprecision(PORTA, 4); break;
				case 127: value=value>>3; break;
			}
			if(*values!=value) *values=value|MODIFIED;
		}
	}
}
