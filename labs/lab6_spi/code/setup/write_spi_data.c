/*ECED3204 Setup code for Lab #6. Not required by students. You must set the 'secret' random seed. */
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
void write_byte(uint16_t addr, uint8_t data);
void setup_eeprom(void);

static int uart_putchar(char c, FILE *stream);
static int uart_getchar(FILE *stream);
FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
FILE mystdin = FDEV_SETUP_STREAM(NULL, uart_getchar, _FDEV_SETUP_READ);

#define RANDOM_SEED ????

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

#define CS_HIGH() PORTB |= 1<<4
#define CS_LOW()  PORTB &= ~(1<<4)

#define INST_WREN  0x06 /* Write Enable */
#define INST_WRDI  0x04 /* Disable writes */
#define INST_RDSR  0x05 /* Read status/config register */
#define INST_WRSR  0x01 /* Write status/config register */
#define INST_READ  0x03 /* Read data from memory */
#define INST_WRITE 0x02 /* Write data to memory */

uint8_t spi_readwrite(uint8_t data)
{
	SPDR = data;	
	loop_until_bit_is_set(SPSR, SPIF);
	return SPDR;
}

int main(void)
{
	init_uart();	
	printf_P(PSTR("System Booted, built %s on %s\n"), __TIME__, __DATE__);
	
	//Set CS as output, other lines set automatically
	DDRB |= (1<<4) | (1<<5) | (1<<7);
	CS_HIGH();
	
	//Slowest possible SPI clock (easiest with long wires)
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR1) | (1<<SPR0);
	
	uint8_t data;
	
	CS_LOW();
	spi_readwrite(INST_RDSR);
	data = spi_readwrite(0);
	CS_HIGH();
	
	printf("Status register: %02x\n", data);
	
	CS_LOW();
	spi_readwrite(INST_READ);
	spi_readwrite(0);
	spi_readwrite(105);
	data = spi_readwrite(0);
	CS_HIGH();
	
	printf("Address %02x: %02x\n", 0x0F, data);	
	
	setup_eeprom();
	printf("EEPROM Setup Done\n");	
}

void setup_eeprom(void)
{
	/* Known addresses */
	write_byte(100, 0x00);
	write_byte(101, 0x01);
	write_byte(102, 0x02);
	write_byte(103, 0x03);
	write_byte(104, 0xDE);
	write_byte(105, 0xAD);
	write_byte(106, 0xBE);
	write_byte(107, 0xEF);
	
	
	//Random seed - this is NOT what has been actually used
	//so you can't cheat on us
	srand(RANDOM_SEED);
	
	/* Secret Addresses */
	for(uint16_t i = 0; i < 100; i++){
		write_byte(i, rand());
	}
}

void write_byte(uint16_t addr, uint8_t data)
{
	CS_LOW();
	spi_readwrite(INST_WREN);
	CS_HIGH();
	
	CS_LOW();
	spi_readwrite(INST_WRITE);
	spi_readwrite(0);
	spi_readwrite(addr);
	spi_readwrite(data);
	CS_HIGH();
	
	//TODO: Just uses fixed delay rather than correctly doing this
	_delay_ms(100);
}