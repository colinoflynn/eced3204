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

unsigned char read_button(unsigned char row, unsigned char col)
{
	unsigned char buttonstate = 0;
	
	//Turn pull-ups on
	PORTD = 0xF0;
	
	//Set row-col low
	DDRD |= 1<<row;
	
	//We need to wait for this to propagate before reading!
	_delay_us(10);
	
	if((PIND & (1<<(col+4))) == 0){
		_delay_ms(20);
		if((PIND & (1<<(col+4))) == 0){
			buttonstate = 1;
		}
	}
	
	DDRD &= ~(1<<row);
	
	return buttonstate;
}

int main(void)
{
	while(1){
		for(unsigned char row = 0; row < 4; row++){
			if (read_button(row, 0)){
				set_led(row);
			}
		}
	}
}