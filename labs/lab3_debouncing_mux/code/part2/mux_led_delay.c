#include <avr/io.h>
#include <util/delay.h>

void set_led(unsigned char lednum)
{
	switch(lednum){
		case 0:
			DDRB = 1<<0 | 1<<2;
			PORTB = 1<<0;
			break;
			
		case 1:
			DDRB = 1<<1 | 1<<2;
			PORTB = 1<<1;
			break;
			
		case 2:
			DDRB = 1<<0 | 1<<3;
			PORTB = 1<<0;
			break;
			
		case 3:
			DDRB = 1<<1 | 1<<3;
			PORTB = 1<<1;
			break;
	}	
}

int main(void)
{
	while(1){
		set_led(0);
        _delay_ms(2000);
        set_led(1);
        _delay_ms(2000);
        set_led(2);
        _delay_ms(2000);
        set_led(3);
        _delay_ms(2000);			
	}
}