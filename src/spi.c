#include "spi.h"
#include <iom2560.h>

// SPI master init
void spi_init_master(void){
    // Set MOSI SCK as OUTPUT
    DDRB = (1<<PB2)|(1<<PB1);

    // Enable SPI, set as master
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

// Transmit data over SPI
uint8_t spi_tranciver(uint8_t data){
    // Load data into buffer
    SPDR = data;

    // Wait for compl transmission
    while(!(SPSR &(1<<SPIF) ));

    // Return rec data
    return(SPDR);
}

// Chip Select pin control
void spi_chip_select(uint8_t select){
    if(select) {
        PORTB &= ~(1<<PB0);
    }else{
        PORTB |= (1 <<PB0);
    }
}
