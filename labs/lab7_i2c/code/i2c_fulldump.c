/*ECED3204 Sample - Lab #7. By Colin O'Flynn, released into public domain */

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

static int uart_putchar(char c, FILE *stream);
static int uart_getchar(FILE *stream);
FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE mystdin = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

static int uart_putchar(char c, FILE *stream)
{
	loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	return 0;
}

static int uart_getchar(FILE *stream)
{
	loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
	return UDR0;
}

void init_uart(void)
{
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	UBRR0 = 7;
	stdout = &mystdout;
	stdin = &mystdin;
}


/* Generic I2C Routines */

void TWI_Start(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	loop_until_bit_is_set(TWCR, TWINT);
}

void TWI_Stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
	loop_until_bit_is_clear(TWCR, TWSTO);
}

void TWI_sendByte(uint8_t cx)
{
	TWDR = cx;
	TWCR = (1<<TWINT)|(1<<TWEN);
	loop_until_bit_is_set(TWCR, TWINT);
}

uint8_t TWI_readByte(char sendAck)
{
	if(sendAck){
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	} else {
		TWCR = (1<<TWINT)|(1<<TWEN);
	}
	loop_until_bit_is_set(TWCR, TWINT);
	return TWDR;
}

uint8_t TWI_status(void)
{
	return TWSR & 0xF8;
}

/* EEPROM Specific Routines - NO error handling done! */

void writePoll(uint8_t SLA)
{
	char busy = 1;
	while(busy){
		TWI_Start();
		TWI_sendByte(SLA);
		if(TWI_status() == 0x18){
			//OK
			busy = 0;
		}
	}
}

void writeByteEE(uint8_t SLA, uint8_t addr, uint8_t data)
{
	TWI_Start();
	TWI_sendByte(SLA);
	TWI_sendByte(addr);
	TWI_sendByte(data);
	TWI_Stop();
	writePoll(SLA);
}

uint8_t readByteEE(uint8_t SLA, uint8_t addr)
{
	uint8_t tmp;
	TWI_Start();
	TWI_sendByte(SLA);
	TWI_sendByte(addr);
	TWI_Start();
	TWI_sendByte(SLA | 0x01);
	tmp = TWI_readByte(0);
	TWI_Stop();
	return tmp;
}

//You can extend these to have error handling - see http://www.embedds.com/programming-avr-i2c-interface/
//for example

#define EEPROM_ADDR 0xA0

int main(void)
{
	init_uart();
	printf_P(PSTR("System Booted, built %s on %s\n"), __TIME__, __DATE__);

	//~50 kHz I2C frequency (slower than normal)
	TWBR = 132;	
	TWCR = 1<<TWEN;
	TWSR = 0;	
	
	uint16_t addr = 105;	
	printf("Read address 0x%02x = %02x\n", addr, readByteEE(EEPROM_ADDR, addr));
	
}
