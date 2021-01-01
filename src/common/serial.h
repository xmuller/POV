# pragma once

#define BAUD 38400
#define MYUBRR F_CPU/16/BAUD-1

void USART_Init(unsigned int ubrr);

void USART_Transmit(unsigned char data);
void USART_Transmit_String(const char* s);

int USART_Receive(unsigned char* c);
int USART_Receive_String(unsigned char* buffer, unsigned int len, unsigned int timeout);
int USART_Available(unsigned char* buffer, unsigned int len);

/* #include <stdarg.h> */
/* void USART_Transmit_String(unsigned char* s, ...); */
