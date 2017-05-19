// ATMega328

#include <SPI.h>
#include <MCP3008.h>

// SC16IS752_SPI for a converter between SPI and a Serial port
#include "mbed.h"
#include "SC16IS752.h"
 
SPI spi(PTD2, PTD3, PTD1); //MOSI, MISO, SCK
SC16IS752_SPI serial_bridge(&spi, PTD0);  // SPI port and CS pin
 
Serial pc(USBTX,USBRX);
 
int main() {
  pc.printf("\nHello World!\n");
 
  serial_bridge.baud(9600);
  //serial_bridge.printf("\nHello World!\n"); // supported through Stream
  
  while(1) { 
    serial_bridge.ioSetState(0x00);
    wait(0.5);
    serial_bridge.ioSetState(0xFF); 
    wait(0.5); 
    serial_bridge.putc('*');  
    pc.putc('*');                
  }
}

MCP button_pannel(1);

#define	MODIFIED	0x80

// ***** Potentiometer boards *****
// Pin 23-24 (PC0-PC1) : select the 4051 channel, hence the board
#define POT_BOARD	PORTC
// Highest board number
#define MAX_POT_BOARD	0x05
// Highest channel number on the highest board
#define MAX_POT_CHANNEL	0x07



// ***** SPI control *****
// Port B0-2 (Pin 12-14) is used to select the channel (ie SPI device) on 74LS138
// ARDUINO MINI PORT B
//	Pin 12: port B bit 0
//	Pin 13: port B bit 1
//	Pin 14: port B bit 2
//	Pin 15: port B bit 3 / SPI MOSI
//	Pin 16: port B bit 4 / SPI MISO
//	Pin 17: port B bit 5 / SPI SCK
//	Pin  7: port B bit 6 / XTAL1
//	Pin  8: port B bit 7 / XTAL2

#define SPI_DEVICE_IDLE	0
#define SPI_DEVICE_ADC		1
#define SPI_DEVICE_BUTTON	2
#define SPI_DEVICE_LED		3
#define SPI_DEVICE_MIDI	4
#define SPI_DEVICE_RAM		5
#define	SPI_CS_PIN	12
#define SPI_CLOCK_PIN   9
#define SPI_MOSI_PIN    11
#define SPI_MISO_PIN    10

char values[28];

void spi_select(byte value)
{
	PORTB &= 0xf8;
	if(value==SPI_DEVICE_IDLE)
	{
		SPI.endTransaction();
		digitalwrite(SPI_SELECT, HIGH);
	} else {
		PORTB |= (value & 0x05);
		SPI.beginTransaction();
		digitalwrite(SPI_SELECT, LOW);
	}
}

void setup()
{
	// Port B (Pin 14-16) is used to select the channel (ie SPI device) on 74LS138
	DDRB |= 0x05;	// Set pins 14-16 of Port B as OUTPUT
	...
	SPI.begin();


	pinMode(23, OUTPUT);	// Port C.0
	pinMode(24, OUTPUT);	// Port C.1

						
	...
}
void loop()
{
	pot_scan();
}
void pot_scan(char *values)
{
	int board, channel, value;
	
	
	serial_bridge.ioSetState(0x00);
	wait(0.5);
	serial_bridge.ioSetState(0xFF);
	wait(0.5);
	serial_bridge.putc('*');
	pc.putc('*');

	spi_select(SPI_DEVICE_ADC);
	digitalwrite(SPI_SELECT, LOW);
	SPI.beginTransaction(SPISettings(3600000, MSBFIRST, SPI_MODE0))

	for (board=0; board<MAX_POT_BOARD; board++)
	{
		digitalwrite(POT_BOARD, SPI_ADC);
		for (channel=0; channel<MAX_POT_CHANNEL; channel++, values++)
		{
			SPI.transfer(channel);
			SPI.transfer(channel);
			value=spi.transfer(0x00);
			if(*values!=value) *values=value|MODIFIED;
		}
	}
	SPI.endTransaction();
	digitalwrite(SPI_SELECT, HIGH);
}


