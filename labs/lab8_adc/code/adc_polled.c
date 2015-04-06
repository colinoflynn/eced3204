/*ECED3204 Sample - Lab #8. By Colin O'Flynn, released into public domain */

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#define F_CPU 14745600UL
#include <util/delay.h>

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


int main(void)
{
	init_uart();
	printf_P(PSTR("System Booted, built %s on %s\n"), __TIME__, __DATE__);
	
	ADCSRA = 0x87;
	ADMUX = 0x40;
	ADCSRB = 0;
	DIDR0 = 0xFE;
	DIDR1 = 0xFF;
	
	int mv_reading;
	
	while(1){
		ADCSRA |= 1<<ADSC;
		loop_until_bit_is_set(ADCSRA, ADIF);
		ADCSRA |= 1<<ADIF;	
				
		mv_reading = ((float)ADC / 1024.0) * 5.0 * 1000.0;

		//Q1 to students: Why doesn't this work?:
		//mv_reading = (ADC / 1024) * 5 * 1000;
		
		//Q2 to students: Why doesn't this work?
		//mv_reading = ((ADC * 1000 * 5) / 1024;
		
		//Q3 to students: Making the above work, what happens to code size?
		
		printf("Reading = %d mV\n", mv_reading);				
		_delay_ms(500);
	}		
}
