#include "serial.h"

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "config.h"

namespace pov::serial
{
  static char print_buffer[config.PRINT_BUFFER_SIZE];

  void init(){
    constexpr unsigned int ubrr = internal::calculateUBRR(config.BAUD_RATE, config.MODE);
    static_assert (ubrr < 4096 && ubrr >= 0,
                    "ERROR UBRR overflow, its value must be less than 4096, please check your baud rate.");

    /*Set baud rate */
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;

    // U2X0
    UCSR0A = UCSR0A & static_cast<uint8_t>(~(1<<U2X0));

    /* Enable receiver and transmitter + receive complete interrupt*/
    // Alternate Function (AF)
    UCSR0B = (1<<RXEN0)|(1<<TXEN0) | ( 1 << RXCIE0) ;

    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
  }

  void transmit(char data){
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)));

    UDR0 = (uint8_t)data;
  }

  void transmit(const char* s){
    while( *s != '\0') {
      transmit(*s);
      s++;
    }
  }

  char receive()
  {
    /* Wait for data to be received */
    while (!dataAvailable())
    {}

    if(warning_enabled) {
      /* Put data into buffer, sends the data */
      if (UCSR0A & (1 << FE0))
        serial::transmit("Frame error\n");
      if (UCSR0A & (1 << DOR0))
        serial::transmit("Data overrun\n");
      if (UCSR0A & (1 << UPE0))
        serial::transmit("Parity error\n");
    }
    /* Get and return received data from buffer */
    return (char)UDR0;
  }

  void printf(const char *fmt, ...)
  {
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(print_buffer, config.PRINT_BUFFER_SIZE, fmt, argptr);
    va_end(argptr);
    transmit(print_buffer);
  }

  bool dataAvailable()
  {
    return (UCSR0A & ( 1 << RXC0));
  }

  ISR(USART_RX_vect) {
    char c = serial::receive();
    if( !run_shell ) {
      io_buffer.push(c);
      if ( c == '\n' )
        run_shell = true;
    }
  }
}
