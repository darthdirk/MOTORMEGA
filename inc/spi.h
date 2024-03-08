#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

// SPI master init
void spi_init_master(void);

// Transmit data over SPI
uint8_t spi_tranciver(uint8_t data);

// Chip Select pin control
void spi_chip_select(uint8_t select);

#endif // SPI_H_