#include <avr/io.h>

int main(void)
{
	DDRD = 1<<7;	
	PORTB = 1<<4;
	
	unsigned char ledstate = 1;
	unsigned char buttonstate = 0;
	unsigned char lastbuttonstate = 0;
	
	while(1)
	{
		buttonstate = PINB & (1<<4);
		
		if ((buttonstate != lastbuttonstate) && (buttonstate == 0)){
			ledstate ^= 1;			
		}
		
		lastbuttonstate = buttonstate;
		
		if(ledstate) {			
			PORTD = 1<<7;
		} else {
			PORTD = 0;
		}
	}
}