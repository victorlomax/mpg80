# mpg80
Recreation of a Roland MPG80 with an Arduino.

Characteristics:
- 29 linear sliders (10K)
- 7 potentiometers (10K)
- 64 LEDS
- 32 tactile switches
- HD44780 compatible 16x2 LCD display

Chips used:
- Atmel ATMega328P (CPU)
- Microchip MCP (SPI compatible Dual channel UART; MIDI Interface)
- Maxim Integrated MAX7221 (SPI compatible LED driver)
- NXP Semiconductor SC16IS752 (SPI compatible dual-channel 8 bits GPIO; Switch driver)
- Microchip MCP3008 (SPI compatible 8-channel 10 bits ADC)
- 4051 (8 channel analog switch; select which group of sliders/potentiometers to scan)
- 747139 (dual 2-in-4 decoder; select SPI device)
 
ATMega pins used:
- PD0-PD7 (2-6, 11-13): communication with LCD display
- PB0, PB1 (14, 15): drives 74139 to select SPI device
- PB2 (16): drives 74139 pin /E to enable/disable SPI communication
- PB3-PB5 (17-19): SPI communication
- PC0-PC2 (23-25): drives 4051 to power a group of potentiometers
- PD2 (4): receives interrupt from SC16IS752 when a switch is pressed
