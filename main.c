#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>  
#include <util/delay.h>
#include <avr/interrupt.h>
#include "step.h"
#include "McuPinDef.h"
#include "spi.h"
#include "usart.h"

#define F_CPU 16000000UL // assuming a 16MHz clock, adjust as necessary
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD - 1
#define STEPS_PER_REV 200
#define DELAY_MS 100


// TIMER1 compare interrupt service routine
// fires when counter value matches OCR1A
ISR(TIMER1_COMPA_vect){
	PORTH ^= _BV(PORTH4);
}



int main(int argc, const char** argv) {
    step_init();
    usart_init(MYUBRR);
    spi_init_master();
    spi_chip_select(0);
    uint8_t target_position = 100;
    
    while (1) {    
        spi_chip_select(1);
        uint8_t received_data = spi_tranciver(0xFF); // receive encoder position 
        spi_chip_select(0);

        if(received_data < target_position){
            step_fwd();
        } else if(received_data > target_position) {
            step_rev();
        }
        debug_output(received_data);
    }
    return 0;
}