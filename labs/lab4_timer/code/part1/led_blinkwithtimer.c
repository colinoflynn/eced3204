/*ECED3204 Sample - Lab #4, Part 1. By Colin O'Flynn, released into public domain */

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned int tick;

#define LED_ON() PORTD |= 1<<7
#define LED_OFF() PORTD &= ~(1<<7)

int main(void)
{
    DDRD |= 1<<7;
	
	//Timer configuration	
	TCCR0A = 0;
	TCCR0B = (1 << CS00) | (1 << CS01);
	
	//Interrupt mask enables
	TIMSK0 |= 1 << TOIE0;
	
	//Enable global interrupts
	sei();	
	
	while(1);	
}

ISR(TIMER0_OVF_vect)
{
	tick++;

	if(tick == 1){
		LED_ON();
	} else if (tick == 400){
		LED_OFF();
	} else if (tick > 900){
		tick = 0;
	}
	
}