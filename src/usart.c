#include "McuPinDef.h"
#include "usart.h"
#include <iom2560.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 64
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD -1


volatile uint8_t data_buffer[BUFFER_SIZE] = {0};
volatile uint8_t data_in = 0;
volatile uint8_t data_out = 0;
void process_data(uint8_t data);

// ISR for USART1 data reception
ISR(USART1_RX_vect) {
    // Read data
    uint8_t received_data = UDR1;

    // Check buffer
    if ((data_in + 1) % BUFFER_SIZE != data_out) {
        // Store data in the buffer
        data_buffer[data_in] = received_data;
        data_in = (data_in + 1) % BUFFER_SIZE;

        // Process data
        debug_output(received_data);
    } 
}

// USART1 initialization
void usart_init(unsigned int ubrr) {
    // Set baud rate for USART1
    UBRR1H = (unsigned char)(ubrr >> 8);
    UBRR1L = (unsigned char)ubrr;
    // Enable receiver and transmitter for USART1
    UCSR1B = (1 << RXEN1) | (1 << TXEN1);
    // Set frame format: 8data, 1stop bit for USART1
    UCSR1C = (1 << UCSZ11) | (1 << UCSZ10); // Correctly sets 8 data bits, and assumes 1 stop bit by default.
    
    usart_transmit_string("USART1 configured\r\n");
}



void usart_transmit(unsigned char data) {
    // Wait for empty transmit buffer
    while (!(UCSR1A & (1 << UDRE1)));
    // Put data into buffer, sends the data
    UDR1 = data;
}


void usart_transmit_string(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        usart_transmit(str[i]);
    }
}

void debug_output(uint8_t data) {
		
	char debug_message[50];
    snprintf(debug_message, sizeof(debug_message), "Encoder Position: %u\r\n", data);
    usart_transmit_string(debug_message);
	
}

void usart_fini();