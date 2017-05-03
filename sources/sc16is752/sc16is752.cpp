/* 
	Description: NXP SC16IS752/SC16IS752 library

Dual UART with I2C-bus/SPI interface, 64-byte FIFOs, IrDA SIR

	Licence: CC BY-NC-SA 3.0

	Version: 0.1

	Release Date: 2017-04-27

	Author: Francois Basquin
	Original author: Tiequan Shao (info@sandboxelectronics.com)

Please keep the above information when you use this code in your project.
*/


//#define SC16IS752_DEBUG_PRINT
#include <SC16IS752.h>
#include <SPI.h>
#include <Wire.h>


#ifdef __AVR__
 #define WIRE Wire
#else // Arduino Due
 #define WIRE Wire1
#endif


// ***** Class constructor
SC16IS752::SC16IS752(byte prtcl, byte addr_sspin)
{
    protocol = prtcl;
    if ( protocol == SC16IS752_PROTOCOL_I2C )
	{ device_address_sspin = (addr_sspin>>1); }
	else
	{ device_address_sspin = addr_sspin; }
	peek_flag = 0;
//	timeout = 1000;
}

void SC16IS752::begin(uint32_t baud)
{
	switch (protocol)
	{
		case SC16IS752_PROTOCOL_I2C:
			WIRE.begin();
			break;
		case SC16IS752_PROTOCOL_SPI:
		default:
			::pinMode(device_address_sspin, OUTPUT);
			::digitalWrite(device_address_sspin, HIGH);
			SPI.setDataMode(SPI_MODE0);
			SPI.setClockDivider(SPI_CLOCK_DIV4);
			SPI.setBitOrder(MSBFIRST);
			SPI.begin();
			break;
	};
	ResetDevice();
	FIFOEnable(1);
	SetBaudrate(baud);
	SetLine(8,0,1);
}

int SC16IS752::available(void)
{
    return FIFOAvailableData();
}

int SC16IS752::read(void)
{
	if ( peek_flag == 0) {
		return ReadByte();
	} else {
		peek_flag = 0;
		return peek_buf;
	}
}

size_t SC16IS752::write(byte val)
{
    WriteByte(val);
}

void SC16IS752::pinMode(byte pin, byte i_o)
{
    GPIOSetPinMode(pin, i_o);
}

void SC16IS752::digitalWrite(byte pin, byte value)
{
    GPIOSetPinState(pin, value);
}

byte SC16IS752::digitalRead(byte pin)
{
   return GPIOGetPinState(pin);
}


byte SC16IS752::ReadRegister(byte channel, byte reg_addr)
{
	byte result;
	switch (protocol)
	{
		case SC16IS752_PROTOCOL_I2C:
			WIRE.beginTransmission(device_address_sspin);
			WIRE.write((reg_addr<<3)|(channel!=0)?0x02:0x00);
			WIRE.endTransmission(0);
			WIRE.requestFrom(device_address_sspin,(byte)1);
			result = WIRE.read();
			break;
		case SC16IS752_PROTOCOL_SPI:
		default:
			::digitalWrite(device_address_sspin, LOW);
			delayMicroseconds(10);
			SPI.transfer((reg_addr<<3)|(channel!=0)?0x82:0x80);
			result = SPI.transfer(0xff);
			delayMicroseconds(10);
			::digitalWrite(device_address_sspin, HIGH);
	}
	return result;
}

void SC16IS752::WriteRegister(byte channel, byte reg_addr, byte val)
{
	switch (protocol)
	{
		case SC16IS752_PROTOCOL_I2C:
			WIRE.beginTransmission(device_address_sspin);
			WIRE.write((reg_addr<<3)|(channel!=0)?0x02:0x00);
			WIRE.write(val);
			WIRE.endTransmission(1);
			break;
		case SC16IS752_PROTOCOL_SPI:
		default:
			::digitalWrite(device_address_sspin, LOW);
			delayMicroseconds(10);
			SPI.transfer((reg_addr<<3)|(channel!=0)?0x02:0x00);
			SPI.transfer(val);
			delayMicroseconds(10);
			::digitalWrite(device_address_sspin, HIGH);
	}
}

int16_t SC16IS752::SetBaudrate(byte channel, uint32_t baudrate) //return error of baudrate parts per thousand
{
    uint16_t divisor;
    byte prescaler;
    uint32_t actual_baudrate;
    int16_t error;
    byte temp_lcr;
    if ( (ReadRegister(channel,SC16IS752_REG_MCR)&0x80) == 0) { //if prescaler==1
        prescaler = 1;
    } else {
        prescaler = 4;
    }

    divisor = (SC16IS752_CRYSTCAL_FREQ/prescaler)/(baudrate*16);

    temp_lcr = ReadRegister(channel,SC16IS752_REG_LCR);
    temp_lcr |= 0x80;
    WriteRegister(channel,SC16IS752_REG_LCR,temp_lcr);
    //write to DLL
    WriteRegister(channel,SC16IS752_REG_DLL,(byte)divisor);
    //write to DLH
    WriteRegister(channel,SC16IS752_REG_DLH,(byte)(divisor>>8));
    temp_lcr &= 0x7F;
    WriteRegister(channel,SC16IS752_REG_LCR,temp_lcr);


    actual_baudrate = (SC16IS752_CRYSTCAL_FREQ/prescaler)/(16*divisor);
    error = ((float)actual_baudrate-baudrate)*1000/baudrate;
#ifdef  SC16IS752_DEBUG_PRINT
    Serial.print("Desired baudrate: ");
    Serial.println(baudrate,DEC);
    Serial.print("Calculated divisor: ");
    Serial.println(divisor,DEC);
    Serial.print("Actual baudrate: ");
    Serial.println(actual_baudrate,DEC);
    Serial.print("Baudrate error: ");
    Serial.println(error,DEC);
#endif

    return error;

}

void SC16IS752::SetLine(byte channel, byte data_length, byte parity_select, byte stop_length )
{
    byte temp_lcr;
    temp_lcr = ReadRegister(channel,SC16IS752_REG_LCR);
    temp_lcr &= 0xC0; //Clear the lower six bit of LCR (LCR[0] to LCR[5]
#ifdef  SC16IS752_DEBUG_PRINT
    Serial.print("LCR Register:0x");
    Serial.println(temp_lcr,DEC);
#endif
    switch (data_length) {            //data length settings
        case 5:
            break;
        case 6:
            temp_lcr |= 0x01;
            break;
        case 7:
            temp_lcr |= 0x02;
            break;
        case 8:
            temp_lcr |= 0x03;
            break;
        default:
            temp_lcr |= 0x03;
            break;
    }

    if ( stop_length == 2 ) {
        temp_lcr |= 0x04;
    }

    switch (parity_select) {            //parity selection length settings
        case 0:                         //no parity
             break;
        case 1:                         //odd parity
            temp_lcr |= 0x08;
            break;
        case 2:                         //even parity
            temp_lcr |= 0x18;
            break;
        case 3:                         //force '1' parity
            temp_lcr |= 0x03;
            break;
        case 4:                         //force '0' parity
            break;
        default:
            break;
    }

    WriteRegister(channel,SC16IS752_REG_LCR,temp_lcr);
}

void SC16IS752::GPIOSetPinMode(byte channel, byte pin_number, byte i_o)
{
    byte temp_iodir;

    temp_iodir = ReadRegister(channel,SC16IS752_REG_IODIR);
    if ( i_o == OUTPUT ) {
      temp_iodir |= (0x01 << pin_number);
    } else {
      temp_iodir &= (byte)~(0x01 << pin_number);
    }

    WriteRegister(channel,SC16IS752_REG_IODIR, temp_iodir);
    return;
}

void SC16IS752::GPIOSetPinState(byte channel, byte pin_number, byte pin_state)
{
    byte temp_iostate;

    temp_iostate = ReadRegister(channel,SC16IS752_REG_IOSTATE);
    if ( pin_state == 1 ) {
      temp_iostate |= (0x01 << pin_number);
    } else {
      temp_iostate &= (byte)~(0x01 << pin_number);
    }

    WriteRegister(channel,SC16IS752_REG_IOSTATE, temp_iostate);
    return;
}


byte SC16IS752::GPIOGetPinState(byte channel, byte pin_number)
{
    byte temp_iostate;

    temp_iostate = ReadRegister(channel,SC16IS752_REG_IOSTATE);
    if ( temp_iostate & (0x01 << pin_number)== 0 ) {
      return 0;
    }
    return 1;
}

byte SC16IS752::GPIOGetPortState(byte channel)
{

    return ReadRegister(channel,SC16IS752_REG_IOSTATE);

}

void SC16IS752::GPIOSetPortMode(byte channel, byte port_io)
{
    WriteRegister(channel,SC16IS752_REG_IODIR, port_io);
    return;
}

void SC16IS752::GPIOSetPortState(byte channel, byte port_state)
{
    WriteRegister(channel,SC16IS752_REG_IOSTATE, port_state);
    return;
}

void SC16IS752::SetPinInterrupt(byte channel, byte io_int_ena)
{
    WriteRegister(channel,SC16IS752_REG_IOINTENA, io_int_ena);
    return;
}

void SC16IS752::ResetDevice(void)
{
	byte reg;

	reg = ReadRegister(0,SC16IS752_REG_IOCONTROL);
	WriteRegister(0,SC16IS752_REG_IOCONTROL, reg|0x08);

	reg = ReadRegister(1,SC16IS752_REG_IOCONTROL);
	WriteRegister(1,SC16IS752_REG_IOCONTROL, reg|0x08);
	return;
}

void SC16IS752::ModemPin(byte channel, byte gpio) //gpio == 0, gpio[7:4] are modem pins, gpio == 1 gpio[7:4] are gpios
{
	byte temp_iocontrol;

	temp_iocontrol = ReadRegister(channel,SC16IS752_REG_IOCONTROL);
	if ( gpio == 0 ) {
        	temp_iocontrol |= 0x02;
	} else {
        	temp_iocontrol &= 0xFD;
	}
	WriteRegister(channel,SC16IS752_REG_IOCONTROL, temp_iocontrol);

	return;
}

void SC16IS752::GPIOLatch(byte channel, byte latch)
{
	byte temp_iocontrol;

	temp_iocontrol = ReadRegister(channel,SC16IS752_REG_IOCONTROL);
	if ( latch == 0 ) {
		temp_iocontrol &= 0xFE;
	} else {
		temp_iocontrol |= 0x01;
	}
	WriteRegister(channel,SC16IS752_REG_IOCONTROL, temp_iocontrol);

	return;
}

void SC16IS752::InterruptControl(byte channel, byte int_ena)
{
	WriteRegister(channel,SC16IS752_REG_IER, int_ena);
}

byte SC16IS752::InterruptPendingTest(byte channel)
{
	return (ReadRegister(channel,SC16IS752_REG_IIR) & 0x01);
}

void SC16IS752::__isr(byte channel)
{
	byte irq_src;

	irq_src = ReadRegister(channel,SC16IS752_REG_IIR);
	irq_src = (irq_src >> 1);
	irq_src &= 0x3F;

	switch (irq_src) {
		case 0x06:		//Receiver Line Status Error
			break;
		case 0x0c:		//Receiver time-out interrupt
			break;
		case 0x04:		//RHR interrupt		
			break;
		case 0x02:		//THR interrupt
			break;
		case 0x00:		//modem interrupt;
			break;
		case 0x30:		//input pin change of state
			break;
		case 0x10:		//XOFF
			break;
		case 0x20:		//CTS,RTS
			break;
		default:
			break;
	}
	return;
}

void SC16IS752::FIFOEnable(byte channel, byte fifo_enable)
{
    byte temp_fcr;

    temp_fcr = ReadRegister(channel,SC16IS752_REG_FCR);

    if (fifo_enable == 0){
        temp_fcr &= 0xFE;
    } else {
        temp_fcr |= 0x01;
    }
    WriteRegister(channel,SC16IS752_REG_FCR,temp_fcr);

    return;
}

void SC16IS752::FIFOReset(byte channel, byte rx_fifo)
{
     byte temp_fcr;

    temp_fcr = ReadRegister(channel,SC16IS752_REG_FCR);

    if (rx_fifo == 0){
        temp_fcr |= 0x04;
    } else {
        temp_fcr |= 0x02;
    }
    WriteRegister(channel,SC16IS752_REG_FCR,temp_fcr);

    return;

}

void SC16IS752::FIFOSetTriggerLevel(byte channel, byte rx_fifo, byte length)
{
    byte temp_reg;

    temp_reg = ReadRegister(channel,SC16IS752_REG_MCR);
    temp_reg |= 0x04;
    WriteRegister(channel,SC16IS752_REG_MCR,temp_reg); //SET MCR[2] to '1' to use TLR register or trigger level control in FCR register

    temp_reg = ReadRegister(channel,SC16IS752_REG_EFR);
    WriteRegister(channel,SC16IS752_REG_EFR, temp_reg|0x10); //set ERF[4] to '1' to use the  enhanced features
    if (rx_fifo == 0) {
        WriteRegister(channel,SC16IS752_REG_TLR, length<<4); //Tx FIFO trigger level setting
    } else {
        WriteRegister(channel,SC16IS752_REG_TLR, length);    //Rx FIFO Trigger level setting
    }
    WriteRegister(channel,SC16IS752_REG_EFR, temp_reg); //restore EFR register

    return;
}

byte SC16IS752::FIFOAvailableData(byte channel)
{
#ifdef  SC16IS752_DEBUG_PRINT
	Serial.print("=====Available data:");
	Serial.println(ReadRegister(channel,SC16IS752_REG_RXLVL), DEC);
#endif
	return ReadRegister(channel,SC16IS752_REG_RXLVL);
//    return ReadRegister(channel,SC16IS752_REG_LSR) & 0x01;
}

byte SC16IS752::FIFOAvailableSpace(void)
{
	return ReadRegister(channel,SC16IS752_REG_TXLVL);
}

void SC16IS752::WriteByte(byte channel, byte val)
{
	byte tmp_lsr;
 /*   while ( FIFOAvailableSpace() == 0 ){
#ifdef  SC16IS752_DEBUG_PRINT
		Serial.println("No available space");
#endif

	};

#ifdef  SC16IS752_DEBUG_PRINT
    Serial.println("++++++++++++Data sent");
#endif
    WriteRegister(channel,SC16IS752_REG_THR,val);
*/
	do {
		tmp_lsr = ReadRegister(channel,SC16IS752_REG_LSR);
	} while ((tmp_lsr&0x20) ==0);

	WriteRegister(channel,SC16IS752_REG_THR,val);



}

int SC16IS752::ReadByte(byte channel)
{
	volatile byte val;
	if (FIFOAvailableData() == 0) {
#ifdef  SC16IS752_DEBUG_PRINT
	Serial.println("No data available");
#endif
		return -1;

	} else {

#ifdef  SC16IS752_DEBUG_PRINT
	Serial.println("***********Data available***********");
#endif
	  val = ReadRegister(channel,SC16IS752_REG_RHR);
	  return val;
	}


}

void SC16IS752::EnableTransmit(byte channel, byte tx_enable)
{
    byte temp_efcr;
    temp_efcr = ReadRegister(channel,SC16IS752_REG_EFCR);
    if ( tx_enable == 0) {
        temp_efcr |= 0x04;
    } else {
        temp_efcr &= 0xFB;
    }
    WriteRegister(channel,SC16IS752_REG_EFCR,temp_efcr);

    return;
}

byte SC16IS752::ping(byte channel)
{
	WriteRegister(channel,SC16IS752_REG_SPR,0x55);
	if (ReadRegister(channel,SC16IS752_REG_SPR) !=0x55) {
		return 0;
	}

	WriteRegister(channel,SC16IS752_REG_SPR,0xAA);
	if (ReadRegister(channel,SC16IS752_REG_SPR) !=0xAA) {
		return 0;
	}

	return 1;

}
/*
void SC16IS752::setTimeout(uint32_t time_out)
{
	timeout = time_out;
}

size_t SC16IS752::readBytes(char *buffer, size_t length)
{
	size_t count=0;
	int16_t tmp;

	while (count < length) {
		tmp = readwithtimeout();
		if (tmp < 0) {
			break;
		}
		*buffer++ = (char)tmp;
		count++;
	}

	return count;
}

int16_t SC16IS752::readwithtimeout()
{
  int16_t tmp;
  uint32_t time_stamp;
  time_stamp = millis();
  do {
    tmp = read();
    if (tmp >= 0) return tmp;
  } while(millis() - time_stamp < timeout);
  return -1;     // -1 indicates timeout
}
*/

void SC16IS752::flush(byte channel)
{
	byte tmp_lsr;

	do {
		tmp_lsr = ReadRegister(channel,SC16IS752_REG_LSR);
	} while ((tmp_lsr&0x20) ==0);


}

int SC16IS752::peek()
{
	if ( peek_flag == 0 ) {
		peek_buf =ReadByte();
		if (  peek_buf >= 0 ) {
			peek_flag = 1;
		}
	}

	return peek_buf;

}


