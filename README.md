# mpg80
Recreation of a Roland MPG80 with an Arduino.

Characteristics:
- 29 linear sliders (10K)
- 11 potentiometers (10K)
- 60 white LEDS + 1 red LED (power status)
  * Note 1: The max number of simulateously lit white LED is 32. 
- 35 tactile switches, replacing the original sliding selectors
- 1  EastRising ERM802FS-3 8x2 Black-on-White LCD display (HD44780-compatible)
- internal 8 KB memory allowing to store 256 tones and patches
- arpeggiator (similar to the Jupiter 6)
- general power switch controlling the MKS-80 and the MPG-80
  
- 74HC138 (dual 3-in-8 decoder; select SPI device)
  * 000: Select LCD Driver (HD44780)
  * 001: Select ADC (MCP3008)
  * 010: Select GPIO (MCP23S17)
  * 011: Select UART (SC16IS752)
  * 100: Select LED Driver (MAX7221 or AS1106)
  * 101: Select FRAM (MB85RS64VPNF)
  * 110: (not used)
  * 111: (not used)
  

ATMega pins used:
- PD0-PD7 (2-6, 11-13): DB0-DB7 on LCD display
- PC3 (26): R/W on LCD display
- PC4 (27): RS on LCD display
- PC5 (28): enable/disable 74138
- PB0, PB1 (14-16): drives 74138 to select SPI device
- PB3-PB5 (17-19): SPI communication
- PC0-PC2 (23-25): drives 4051 to power a group of potentiometers
- PD2 (4): interrupt from MCP23S17 when a switch is pressed
