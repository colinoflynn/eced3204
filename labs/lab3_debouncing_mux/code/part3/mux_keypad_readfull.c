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
			
		default:
			DDRB = 0;
			PORTB = 0;
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

unsigned char decode_buttons(void)
{
	unsigned char row, col;
	unsigned char button = 0xff;
	
	for(row = 0; row < 4; row++){
		for(col = 0; col < 4; col++){
			if(read_button(row, col)){
				button = row | (col << 4);
			}
		}
	}
	
	
	
	switch(button){
		case (0<<4) | 0: return 0x0F;
		case (0<<4) | 1: return 0x0E;
		case (0<<4) | 2: return 0x0D;
		case (0<<4) | 3: return 0x0C;
		case (1<<4) | 0: return 0x0B;
		case (1<<4) | 1: return 0x0A;
		case (1<<4) | 2: return 0x09;
		case (1<<4) | 3: return 0x08;
		case (2<<4) | 0: return 0x07;
		case (2<<4) | 1: return 0x06;
		case (2<<4) | 2: return 0x05;
		case (2<<4) | 3: return 0x04;
		case (3<<4) | 0: return 0x03;
		case (3<<4) | 1: return 0x02;
		case (3<<4) | 2: return 0x01;
		case (3<<4) | 3: return 0x00;
		default: return 0xFF;
	}
}


int main(void)
{
	unsigned char button, tempbutton;

	while(1){	
		tempbutton = decode_buttons();
	
		if (tempbutton != 0xFF){
			button = tempbutton;
		} else {
			_delay_ms(30);
		}

		switch(button){			
			case 0:
				set_led(0xFF);
				break;
				
			case 1:
				set_led(3);
				break;
				
			case 2:
				set_led(2);
				break;
				
			case 3:
				set_led(3);
				_delay_ms(50);
				set_led(2);
				break;
				
			case 4:
				//FILL IN FROM 4 to 0x0D
				break;
				
			case 0x0D:
				//FILL THIS IN
				break;
				
			case 0x0E:
				set_led(0);
				_delay_ms(50);
				set_led(1);
				_delay_ms(50);
				set_led(2);
				break;
				
			case 0x0F:
				//FILL THIS IN			
				break;	
				
			default:
				break;
		}
	}
}