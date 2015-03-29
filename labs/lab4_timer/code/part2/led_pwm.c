/*ECED3204 Sample - Lab #4, Part 2. By Colin O'Flynn, released into public domain */

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRD |= 1<<7;
	
	//Set clock divider to be /8
	TCCR2B = 1<<CS21;
	
	//Set waveform generation mode
	TCCR2A |= 1<<WGM20;
	
	//Set output on OC2A pin
	TCCR2A |= 1<<COM2A1;
		
	OCR2A = 0;		
	while(1){
		OCR2A++;
		_delay_ms(50);		
	}
}

