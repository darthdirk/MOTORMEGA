#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>  
#include <util/delay.h>
#include <avr/interrupt.h>
#include "step.h"
#include "McuPinDef.h"

 
#define STEPS_PER_REV 200
#define F_CPU 16000000UL // Assuming 16 MHz clock frequency
#define DELAY_MS 100


// TIMER1 compare interrupt service routine
// fires when counter value matches OCR1A
ISR(TIMER1_COMPA_vect){
	PORTH ^= _BV(PORTH4);
}



int main(int argc, const char** argv) {
    step_init();
    
    while (1) {    
        // move the motor forward one step
        step_fwd();
    }
    
    return 0;
}