#include <avr/io.h>
#include <util/delay.h>

#include "timer.h"

#define BAUD 38400
#define MYUBRR F_CPU / 16 / BAUD - 1

void USART_Init(unsigned int ubrr)
{
    /*Set baud rate */
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    // U2X0
    UCSR0A &= ~(1 << U2X0);

    /* Enable receiver and transmitter */
    // Alternate Function (AF)
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1 << USBS0) | (3 << UCSZ00);
}

void USART_Transmit(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1 << UDRE0)))
        ;

    /* Put data into buffer, sends the data */
    UDR0 = data;
}

void USART_Transmit_String(unsigned char *s)
{
    while (*s != '\0')
    {
        USART_Transmit(*s);
        s++;
    }
}

void main()
{
    USART_Init(MYUBRR);
    while (1)
    {
        _delay_ms(500);
        USART_Transmit(time_milli());
    }
}
