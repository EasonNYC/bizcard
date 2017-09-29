/*
 * main.c
 *
 * Created: 8/11/2017 2:28:24 PM
 * Author : EpicZero
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//#include "leds.h"

#define CAPSENSE 0x10		//PB4

volatile uint16_t ledbitarray = 0x0;	//initial LED states

const uint8_t LED_RDY[12] = { 0x9,
	0xA,
	0xC,
	0xC,
	0x5,
	0x6,
	0x6,
	0xA,
	0x3,
	0x3,
	0x5,
	0x9
};

const uint8_t LED[12] = { 0x1,
	0x2,
	0x4,
	0x8,
	0x1,
	0x2,
	0x4,
	0x8,
	0x1,
	0x2,
	0x4,
	0x8
};

static void initTimer(void)
{
	TCCR1 = 0;		//reset
	TCNT1 = 0;		//reset
	GTCCR = (1 << PSR1);
	OCR1A = 0;		//compareA triggers interrupt 125 5
	OCR1C = 14;		//compareC resets updown counter 243 20
	TIMSK = (1 << OCIE1A);	//enable compare match interrupt
	TCCR1 = (1 << CTC1) | (1 << CS12) | (1 << CS11) | (1 << CS10);
	sei();
} 
//cnt incriments roughly every 1ms
volatile uint32_t msticks = 0;

ISR(TIMER1_COMPA_vect)
{

	//always clear matrix
	
	PORTB &= ~0xF;		//(todo: mask capsense pin)
	DDRB &= ~0xF;		//(todo: mask capsense pin)

	uint8_t index = msticks % 12;

	//if led is supposed to be on
	if ((ledbitarray >> index) & 0x1) {
		//configure matrix (todo: mask capsense pin)
		DDRB |= LED_RDY[index];

		//turn LED on (todo: mask capsense pin)
		PORTB |= LED[index];
	}
	
	msticks++;
        
	PORTB ^= 0x10; //toggle pin pb4 for frequency debug
}

uint8_t direction = 1;

int main(void)
{

	initTimer();

	//enable port change interrupts
	//cli();
	//GIMSK |= _BV(PCIE)
        //PCMSK |= 0x10;
	//sei();
	
	DDRB &= ~(0x10);//set capsense pin as as input
	PORTB &= ~(0x10);//turn off pullup resistor to start

	while (1) {

		_delay_ms(25);	
		if(direction)
		{ 
		ledbitarray <<= 0x1; //shift left one position
		ledbitarray |= 0x1; //add one led
		}
		else
		{
		ledbitarray >>= 0x1;
		}
                if(ledbitarray == 0xFFF)
		{
		direction = 0; 
		}
		if(ledbitarray == 0)
		{
		direction = 1;
		} 

		//ledbitarray = (0x1 << (analogresult / 100));
	}
}
