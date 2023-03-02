/*
 * init.c
 *
 * Created: 11/24/2022 11:10:19 AM
 *  Author: Armin
 */ 

#include "init.h"


// Port initialization
void InitPorts() {
	DDRA = 0xFF;                        // 7 segment
	DDRB = 0xF0;                        // LED 0-3
	DDRC = 0x78 | 0x80;                 // Matrix + Red LED
	DDRD = 0xF0;                        // LED 4-7
}


// Timer initialization
void InitTimer() {

	// Timer 0 initialization
	// TIMER0 CTC COMPA 256 10Hz - 100ms
	TCCR0 = (0<<WGM01) | (0<<WGM00) | (1<<CS02) | (1<<CS01) | (0<<CS00);
	TIMSK |= (0<<OCIE0) | (1<<TOIE0);
	
	
	// Timer 1 1mp-es interrupt
	// TIMER1 CTC COMPA 256 10Hz - 100ms
	TCCR1A = (0<<WGM11) | (0<<WGM10);
	TCCR1B = (0<<WGM13) | (1<<WGM12) | (1<<CS12) | (0<<CS11) | (0<<CS10);
	TIMSK |= (1<<OCIE1A);
	OCR1A = 6250-1;



	// Timer 2 initialization
	// TIMER2 CTC COMPA 256 10Hz - 100ms
	TCCR2 = (0<<WGM21) | (0<<WGM20) | (0<<CS22) | (1<<CS21) | (1<<CS20);
	TIMSK |= (0<<OCIE2) | (1<<TOIE2);

	sei();
}