#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD = 1<<7;	
	PORTB = 1<<4;
	
	unsigned char ledstate = 1;
	unsigned char buttonstate = 0;
	unsigned char lastbuttonstate = 0;
	
    unsigned char tempbuttonstate;
    
	while(1)
	{
		tempbuttonstate = PINB & (1<<4);
        _delay_ms(20);
        
        if ((PINB & (1<<4)) == tempbuttonstate) {
            buttonstate = tempbuttonstate;
        }
        
        
		
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