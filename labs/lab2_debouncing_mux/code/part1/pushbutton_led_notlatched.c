#include <avr/io.h>

int main(void)
{
	//PORTD.7 is Output
	//PORTD.4 is Input
	DDRD = 1<<7;
	
	//Turn pull-up on PORTD.4
	PORTB = 1<<4;
	
	while(1){
		//Check state of push-button
		if(PINB & (1<<4)){			
			//Turn on PORTD.7
			PORTD = 1<<7;			
		} else {			
			//Turn off PORTD.7
			PORTD &= ~(1<<7);		
		}		
	}	
}