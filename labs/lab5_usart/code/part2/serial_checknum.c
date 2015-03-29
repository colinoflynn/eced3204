/*ECED3204 Sample - Lab #5, Part 2. By Colin O'Flynn, released into public domain */

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

int main(void)
{
	init_uart();	
	printf_P(PSTR("System Booted, built %s on %s\n"), __TIME__, __DATE__);
	
	printf_P(PSTR("Hello There. What is your name?\n"));
	
	char name[32];
	
	scanf("%s", name);
	
	printf_P(PSTR("Alright %s. What is a number?\n"), name);
	
	int number;
	
	if (scanf("%d", &number) == 1){
		printf_P(PSTR("OK %s, did you pick %d?\n"), name, number);
	} else {
		printf_P(PSTR("Look %s - I said a number. Try again.\n"));
	}
	
}
