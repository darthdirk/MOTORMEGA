#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>  
#include <util/delay.h>
 
#define PUL_PIN PH4
#define DIR_PIN  PH3
#define STEPS_PER_REV 200
#define F_CPU 16000000UL
//Assuming 16 MHz clock frequency


void init() {
	// set PUL_PIN and DIR_PIN as output
	DDRH |= (1 << PUL_PIN) | (1 << DIR_PIN);
	DDRB |= (1 << PB7) | (1 << PB6);
}


void stepFwd() {
	// Set the direction pin for forward motion
	PORTH |= (1 << DIR_PIN);


	// Generate the pulse on the step pin
	PORTH |= (1 << PUL_PIN);
	PORTB |= (1 << PB7); //Turn on LED on PB7
	_delay_us(500); // Adjust the delay based on the motor specs 
	PORTH &= ~(1 << PUL_PIN);
	PORTB &= ~(1 << PB7); // Turn off LED on PB7
}


void stepRev() {
	// Set the direction pin for the backward motion
	PORTH &= ~(1 << DIR_PIN);
	

	// Generate a pulse on the step pin
	PORTH |= (1 << PUL_PIN);
	PORTB |= (1 << PB6); // Turn on LED on PB6
	_delay_us(500); // Adjust the delay based on the motor specs
	PORTH &= ~(1 << PUL_PIN);
	PORTB &= ~(1 << PB6); // Turn off LED on PB6
}

int main() {
	init();

/*	// Move the motor forward for one revolution
	for (int i = 0; i < STEPS_PER_REV; ++i) {
		stepFwd();
	}

	// Move the motor back one revolution
	for (int i = 0; i < STEPS_PER_REV; ++i){
		stepRev();
	}*/
	while (1) {
		// move the motor forward one step
		stepFwd();

		// Add a delay to control the speed oof the motor
		_delay_us(1);
	}

	return 0;

}


