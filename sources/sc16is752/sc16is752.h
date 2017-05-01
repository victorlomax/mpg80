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

#ifndef SC16IS752_h
#define SC16IS752_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

//Device Address

//A:VDD
//B:GND
//C:SCL
//D:SDA
// *752
#define     SC16IS752_ADDRESS_AA     (0X90)
#define     SC16IS752_ADDRESS_AB     (0X92)
#define     SC16IS752_ADDRESS_AC     (0X94)
#define     SC16IS752_ADDRESS_AD     (0X96)
#define     SC16IS752_ADDRESS_BA     (0X98)
#define     SC16IS752_ADDRESS_BB     (0X9A)
#define     SC16IS752_ADDRESS_BC     (0X9C)
#define     SC16IS752_ADDRESS_BD     (0X9E)
#define     SC16IS752_ADDRESS_CA     (0XA0)
#define     SC16IS752_ADDRESS_CB     (0XA2)
#define     SC16IS752_ADDRESS_CC     (0XA4)
#define     SC16IS752_ADDRESS_CD     (0XA6)
#define     SC16IS752_ADDRESS_DA     (0XA8)
#define     SC16IS752_ADDRESS_DB     (0XAA)
#define     SC16IS752_ADDRESS_DC     (0XAC)
#define     SC16IS752_ADDRESS_DD     (0XAE)


// ***** Registers
//General Registers
#define     SC16IS752_REG_RHR        (0x00)
#define     SC16IS752_REG_THR        (0X00)
#define     SC16IS752_REG_IER        (0X01)
#define     SC16IS752_REG_FCR        (0X02)
#define     SC16IS752_REG_IIR        (0X02)
#define     SC16IS752_REG_LCR        (0X03)
#define     SC16IS752_REG_MCR        (0X04)
#define     SC16IS752_REG_LSR        (0X05)
#define     SC16IS752_REG_MSR        (0X06)
#define     SC16IS752_REG_SPR        (0X07)
#define     SC16IS752_REG_TCR        (0X06)
#define     SC16IS752_REG_TLR        (0X07)
#define     SC16IS752_REG_TXLVL      (0X08)
#define     SC16IS752_REG_RXLVL      (0X09)
#define     SC16IS752_REG_IODIR      (0X0A)
#define     SC16IS752_REG_IOSTATE    (0X0B)
#define     SC16IS752_REG_IOINTENA   (0X0C)
#define     SC16IS752_REG_IOCONTROL  (0X0E)
#define     SC16IS752_REG_EFCR       (0X0F)
//Special Registers
#define     SC16IS752_REG_DLL        (0x00)
#define     SC16IS752_REG_DLH        (0X01)
//Enhanced Registers
#define     SC16IS752_REG_EFR        (0X02)
#define     SC16IS752_REG_XON1       (0X04)
#define     SC16IS752_REG_XON2       (0X05)
#define     SC16IS752_REG_XOFF1      (0X06)
#define     SC16IS752_REG_XOFF2      (0X07)

// ***** Interrupts
#define     SC16IS752_INT_CTS        (0X80)
#define     SC16IS752_INT_RTS        (0X40)
#define     SC16IS752_INT_XOFF       (0X20)
#define     SC16IS752_INT_SLEEP      (0X10)
#define     SC16IS752_INT_MODEM      (0X08)
#define     SC16IS752_INT_LINE       (0X04)
#define     SC16IS752_INT_THR        (0X02)
#define     SC16IS752_INT_RHR        (0X01)

//Application Related 

#define     SC16IS752_CRYSTCAL_FREQ (14745600UL) 
//#define 	SC16IS752_CRYSTCAL_FREQ (1843200UL)	  
//#define     SC16IS752_CRYSTCAL_FREQ (16000000UL)    
//#define     SC16IS752_DEBUG_PRINT   (0)
#define     SC16IS752_PROTOCOL_I2C  (0)
#define     SC16IS752_PROTOCOL_SPI  (1)


// ***** Class

class SC16IS752 : public Stream
{
private:
	byte	device_address_sspin;
        byte	protocol;
	//	uint32_t timeout;
        word	SetBaudrate(uint32_t baudrate);
        byte	ReadRegister(byte channel, byte reg_addr);
        void    WriteRegister(byte channel, byte reg_addr, byte val);
        void    SetLine(byte data_length, byte parity_select, byte stop_length );
        void    GPIOSetPinMode(byte pin_number, byte i_o);
        void    GPIOSetPinState(byte pin_number, byte pin_state);
        byte	GPIOGetPinState(byte pin_number);
        void    GPIOSetPortMode(byte port_io);
        void    GPIOSetPortState(byte port_state);
        void    ResetDevice(void);
        void    __isr(void);
        void    FIFOEnable(byte fifo_enable);
        void    FIFOReset(byte rx_fifo);
        void    FIFOSetTriggerLevel(byte rx_fifo, byte length);
        byte	FIFOAvailableData(void);
        byte	FIFOAvailableSpace(void);
        void    WriteByte(byte val);
        int     ReadByte(void);
        void    EnableTransmit(byte tx_enable);
	//	word readwithtimeout();
	int	peek_buf;
	byte	peek_flag;
public:
	SC16IS752(byte prtcl = SC16IS752_PROTOCOL_I2C, byte addr = SC16IS752_ADDRESS_AD);
	void	begin(uint32_t baud);
	int	read();
	size_t	write(byte val);
	int	available();
	void	pinMode(byte pin, byte io);
	void	digitalWrite(byte pin, byte value);
	byte	digitalRead(byte pin);
	byte	ping();
	//	void setTimeout(uint32_t);
	//	size_t readBytes(char *buffer, size_t length);
	int	peek();
	void	flush();
	byte	GPIOGetPortState(void);
	byte	InterruptPendingTest(void);
	void	SetPinInterrupt(byte io_int_ena);
	void	InterruptControl(byte int_ena);
	void	ModemPin(byte gpio); //gpio == 0, gpio[7:4] are modem pins, gpio == 1 gpio[7:4] are gpios
	void	GPIOLatch(byte latch);
};
#endif





    
    

