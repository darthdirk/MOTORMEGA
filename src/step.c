#include "step.h"
#include "McuPinDef.h"
#include <iom2560.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define PRESCALER 10 // 10bits ... 1024
#define CLOCK_RATE 15998976 // roughly num of ticks per second
#define COUNTER_VALUE (CLOCK_RATE >> PRESCALER)


/*
configures Timer 1 for Fast PWM mode with OCR1A as the top value,
allowing for PWM frequency and duty cycle control. 
The prescaler and OCR1A value should be adjusted for the motor speed and control.
*/
int step_init(){
    // set PUL_PIN and DIR_PIN as output
    DDRH |= (1 << PUL_PIN) | (1 << DIR_PIN);
    
    // Set Fast PWM mode using ICR1 as TOP

	TCCR1A |= (1 << WGM11); 
	TCCR1B |= (1 << WGM12) | (1 << WGM13);

    // Set prescaler to 8 
    TCCR1B |= (1 << CS11);
    TCCR1B &= ~((1 << CS12) | (1 << CS10)); // Clear CS12 and CS10
    // configure timer
	OCR1A= COUNTER_VALUE;

	// I want an interruot
	TIMSK1 |= (1 << OCIE1A);

	//enable interrupts
	sei();

    return 0;
}


int step_fwd() {
    // Set the direction pin for forward motion
    PORTH |= (1 << DIR_PIN);

    // Generate the pulse on the step pins
    PORTH |= (1 << PUL_PIN);
    _delay_us(1); // Adjust the delay based on the motor specs 
    PORTH &= ~(1 << PUL_PIN);
}

int step_rev() {
    // Set the direction pin for the backward motion
    PORTH &= ~(1 << DIR_PIN);
    
    // Generate a pulse on the step pin
    PORTH |= (1 << PUL_PIN);
    _delay_us(1); // Adjust the delay based on the motor specs
    PORTH &= ~(1 << PUL_PIN);
}
