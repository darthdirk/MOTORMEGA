#pragma once

void usart_init(unsigned int ubrr);

void usart_transmit(unsigned char data);

void usart_transmit_string(const char* str);

void debug_output(uint8_t data); 

void usart_fini();
