# mpg80
Recreation of a Roland MPG80 with an Arduino.

Characteristics:
- 29 linear sliders (10K)
- 11 potentiometers (10K)
- 64 white LEDS + 1 red LED (power status)
- 39 tactile switches
- HD44780 compatible 8x2 LCD display

Chips used:
- Atmel ATMega328P (CPU)
- NXP Semiconductor SC16IS752 (SPI compatible Dual channel UART; MIDI Interface)
  * Datasheet: http://www.nxp.com/documents/data_sheet/SC16IS752_SC16IS762.pdf
- Maxim Integrated MAX7221 (SPI compatible LED driver). 
  * Datasheet: https://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf
  * Note 1: The max number on simulateously lit LED is 32. 
  * Note 2: This chip is not a real compatible SPI device as DOUT is not tri-state. 
- Microchip MCP23S17 (SPI compatible dual-channel 8 bits GPIO; Switch driver)
  * Datasheet: http://ww1.microchip.com/downloads/en/DeviceDoc/20001952C.pdf
- Microchip MCP3008 (SPI compatible 8-channel 10 bits ADC)
  * Datasheet: http://ww1.microchip.com/downloads/en/DeviceDoc/21295d.pdf
- 4051 (8 channel analog switch; select which group of sliders/potentiometers to scan)
- 747139 (dual 2-in-4 decoder; select SPI device)
- 6N135 (optocoupler)

ATMega pins used:
- PD0-PD7 (2-6, 11-13): communication with LCD display
- PB0, PB1 (14, 15): drives 74139 to select SPI device
- PB2 (16): drives 74139 pin /E to enable/disable SPI communication
- PB3-PB5 (17-19): SPI communication
- PC0-PC2 (23-25): drives 4051 to power a group of potentiometers
- PD2 (4): receives interrupt from MCP23S17 when a switch is pressed
