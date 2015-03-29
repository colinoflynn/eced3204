/*ECED3204 Sample - Lab #5, Part 1. By Colin O'Flynn, released into public domain */

#include <avr/io.h>
#include <util/delay.h>

void init_uart(void)
{
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	UBRR0 = 7;
}

void write_char(char c)
{	
	//Wait for UDR0 to be ready for writing
	while((UCSR0A & (1<<UDRE0)) == 0);
	
	//Write data byte
	UDR0 = c;
}

char check_char(void)
{
	//Check if data ready for reading
	return (UCSR0A & (1<<RXC0));
}

char read_char(void)
{
	//Wait for data
	while((UCSR0A & (1<<RXC0)) == 0);
	
	//Return data
	return UDR0;
}

int main(void)
{
	init_uart();
	
	
	//STEP1: Just print 'A'	
	while(1){
		write_char('A');
		_delay_ms(500);
	}
	
	//STEP2: Check if character received
	while(1){
		if (check_char()){
			write_char('A');
		} else {
			write_char('B');
		}
		_delay_ms(500);		
	}
	
	//STEP3: Full echo
	while(1){
		write_char(read_char());
	}
	
}
