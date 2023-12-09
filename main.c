#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <avr/io.h>  
#include <util/delay.h>
#include <avr/interrupt.h>
 
#define PUL_PIN PH4
#define DIR_PIN  PH3
#define STEPS_PER_REV 400
#define F_CPU 16000000UL //Assuming 16 MHz clock frequency
#define DELAY_MS 100
#define PRESCALER 10 // 10bits ... 1024
#define CLOCK_RATE 15998976
// roughly the number of timer ticks in a second
#define COUNTER_VALUE (CLOCK_RATE >> PRESCALER)


// TIMER1 compare interrupt service routine
// fires when counter value matches OCR1A
ISR(TIMER1_COMPA_vect){
	PORTH ^= _BV(PORTH4); 
}


void init() {
	// set PUL_PIN and DIR_PIN as output
	DDRH |= (1 << PUL_PIN) | (1 << DIR_PIN);
	
	// configure timer
	OCR1A= COUNTER_VALUE;

	TCCR1A = 0x00;
	TCCR1B |= (1 << WGM12); //CTC mode compare to OCR1A
	TCCR1B |= (1 << CS11) | (1 << CS10); // Set prescales to 32

	// I want an interruot
	TIMSK1 |= (1 << OCIE1A);

	//enable interrupts
	sei();

/*	while(true){
		// do somthing usefull in here while we are waiting for the timer to fire
		}*/
	}

// Function to limit a value within a specified range
uint16_t limitValue(uint16_t value, uint16_t minValue, uint16_t maxValue) {
    if (value < minValue) {
        return minValue;
    } else if (value > maxValue) {
        return maxValue;
    } else {
        return value;
    }
}

void stepFwd() {
	// Set the direction pin for forward motion
	PORTH |= (1 << DIR_PIN);
	// Generate the pulse on the step pin
	PORTH |= (1 << PUL_PIN);
}


void stepRev() {
	// Set the direction pin for the backward motion
	PORTH &= ~(1 << DIR_PIN);
	

	// Generate a pulse on the step pin
	PORTH |= (1 << PUL_PIN);
}

// Function to read the analog value from the potentiometer
uint16_t readPotentiometer() {
	ADMUX = (1 << REFS0) | (0 << MUX0); // Use AVcc as reference voltage
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADSC); // Start conversion
	while (ADCSRA & (1 << ADSC)); // Wait for the version to complete 
	return ADC;
}


// C rep for the map function
uint16_t map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max){
	return (x - in_min) * (out_max -out_min) / (in_max - in_min) + out_min;
}


int main()
{
    init();

	  // Initialize variables to keep track of the previous potValue and speed
    uint16_t prevPotValue = 0;
    uint16_t currentSpeed = 0;

    while (1)
    {
        // Read the analog value from the potentiometer
        uint16_t potValue = readPotentiometer();

        // Calculate the error (difference between current and desired speed)
        int16_t error = potValue - prevPotValue;

        // Use proportional control to adjust the speed
        currentSpeed += error;

        // Limit the speed to avoid extreme values
        currentSpeed = limitValue(currentSpeed, 100, 2000);

        // Set the new COUNTER_VALUE based on the scaled speed
        OCR1A = currentSpeed;

        // Update the previous potValue
        prevPotValue = potValue;

		  // move the motor forward one step
        stepFwd();

        // Do not add a delay here, let the timer handle it

        // Add your other tasks here
    }

    return 0;
}

/*
    while (1)
    {
        // Read the analog value from the potentiometer
        uint16_t potValue = readPotentiometer();

        // Map the potentiometer value to a delay range (adjust as needed)
        uint16_t newCounterValue = map(potValue, 0, 1023, 100, 2000);
		// Calculate a scaling factor based on a power function
		//float scale = pow((potValue / 1023.0), 2); // adjust the exponent as needed
		
		
		//Calculate the new COUNTER_VALUE  based on the scaling factor
		//uint16_t newCounterValue = (uint16_t)(scale * COUNTER_VALUE); 
		
		// Calculate the new COUNTER_VALUE based on potValue
		//uint16_t newCounterValue = map(potValue, 0, 1023, 0, 1000);
		
		OCR1A = newCounterValue;

        // move the motor forward one step
        stepFwd();

        // timer handle delay

        // Add other tasks here
    }

    return 0;
}
*/

