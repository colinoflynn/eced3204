/*ECED3204 Sample - Lab #4, Part 3. By Colin O'Flynn, released into public domain */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <util/delay.h>

#define LED_ON() PORTD |= 1<<7
#define LED_OFF() PORTD &= ~(1<<7)

volatile unsigned char current_edge = 0;
volatile uint16_t      starting_cnt;
volatile uint16_t      ending_cnt;

int main(void)
{
	//LED as output
    DDRD = 1<<7;
	
	//Pull-up on ICP
	PORTD |= 1<<6;
	
	//Defaults
	TCCR1A = 0;
	
	//Enable noise cancel, look for falling edge
	TCCR1B = (1<<ICNC1);
	
	//Clock divided by 1024
	TCCR1B |= (1<<CS12) | (1<<CS10);
	
	//Input capture interrupt
	TIMSK1 |= (1<<ICIE1);
	
	//Enable interrupts
	sei();	
		
	uint16_t timediff;
		
	while(1){
		if(current_edge == 2){			
			//Check for normal (no wrap-around)
			if (starting_cnt < ending_cnt){
				//A - B
				timediff = ending_cnt - starting_cnt;
			} else {
				//wrap around
				//(0xffff + A) - B, done without requiring signed math
				timediff = starting_cnt - ending_cnt;
				timediff = 0xffff - timediff;
			}
			
			if ((timediff > 10000) && (timediff < 30000)){
				LED_ON();
				_delay_ms(5000);
			}
			
			LED_OFF();
			
			current_edge = 0;
		}
	}
}

ISR(TIMER1_CAPT_vect)
{
	if(current_edge == 0){
		//Save timestamp
		starting_cnt = ICR1;
				
		//Switch to rising edge
		TCCR1B |= (1<<ICES1);
		
		current_edge = 1;
		
	} else if (current_edge == 1){		
		//Save timestamp
		ending_cnt = ICR1;
		
		//Switch to falling edge
		TCCR1B &= ~(1<<ICES1);
		
		current_edge = 2;
	}
	
	TIFR1 |= (1<<ICF1);
}