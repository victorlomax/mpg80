# mpg80
Recreation of a Roland MPG80 with an Arduino.

Characteristics:
- 29 linear sliders (10K)
- 11 potentiometers (10K)
- 64 white LEDS + 1 red LED (power status)
  * Note 1: The max number of simulateously lit white LED is 32. 
- 39 tactile switches, replacing the original sliding selectors
- EastRising ERM802FS-3 8x2 Black-on-White LCD display (HD44780-compatible)

Chips used:
- Atmel ATMega328P (CPU)
- NXP Semiconductor SC16IS752 (SPI compatible Dual channel UART; MIDI Interface)
  * Datasheet: http://www.nxp.com/documents/data_sheet/SC16IS752_SC16IS762.pdf
  * 28-TSSOP: Digikey P/N=568-4016-5-ND, Price=5.62 CAD
  
- Maxim Integrated MAX7221 (SPI compatible LED driver). 
  * Datasheet: https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf
  * Note 2: This chip is not a real SPI compatible device as DOUT is not tri-state; however, in our case, we don't use DOUT.
  * 24-SOIC: Digikey P/N=MAX7221CWG+, Price=13.00 CAD
  * 24-DIP: Digikey P/N=MAX7221CNG+-ND, Price=13.18 CAD

- Microchip MCP23S17 (SPI compatible dual-channel 8 bits GPIO; Switch driver)
  * Datasheet: http://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf
  * 28-SOIC: Digikey P/N=MCP23S17T-E/SO, Price=1.78 CAD
  * 28-SPDIP: DigiKey P/N=MCP23S17-E/SP-ND, Price=1.78 CAD
  
- Microchip MCP3008 (SPI compatible 8-channel 10 bits ADC)
  * Datasheet: http://ww1.microchip.com/downloads/en/DeviceDoc/21295d.pdf
  * 16-SOIC: Digikey P/N=MCP3008-I/SL, Price=3.09 CAD
  * 16-DIP: Digikey P/N=	MCP3008-I/P-ND, Price=3.09 CAD
  
- ON Semiconductor 74HC4051 (8 channel analog switch; select which group of sliders/potentiometers to scan)
  * Datasheet: http://www.onsemi.com/pub_link/Collateral/MC74HC4051A-D.PDF
  * 16-SOIC: Digikey P/N=MC74HC4051ADWR2GOSCT-ND, Price=0.85 CAD
  
- 74HC139 (dual 2-in-4 decoder; select SPI device)
- 6N135 (optocoupler)

ATMega pins used:
- PD0-PD7 (2-6, 11-13): communication with LCD display
- PB0, PB1 (14, 15): drives 74139 to select SPI device
- PB2 (16): drives 74139 pin /E to enable/disable SPI communication
- PB3-PB5 (17-19): SPI communication
- PC0-PC2 (23-25): drives 4051 to power a group of potentiometers
- PD2 (4): interrupt from MCP23S17 when a switch is pressed
