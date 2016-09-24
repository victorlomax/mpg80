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
