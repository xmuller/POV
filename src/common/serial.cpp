#include "serial.h"

#include <avr/io.h>
#include <util/delay.h>
#include "config.h"

namespace pov::serial
{
  void init(){
    constexpr unsigned int ubrr = internal::calculateUBRR(config.BAUD_RATE, config.MODE);
    static_assert (ubrr < 4096 && ubrr >= 0,
                    "ERROR UBRR overflow, its value must be less than 4096, please check your baud rate.");

    /*Set baud rate */
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;

    // U2X0
    UCSR0A = UCSR0A & static_cast<uint8_t>(~(1<<U2X0));

    /* Enable receiver and transmitter */
    // Alternate Function (AF)
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);

    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
  }

  void transmit(char data){
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)));

    /* Put data into buffer, sends the data */
    UDR0 = (uint8_t)data;
  }

  void transmit(const char* s){
    while( *s != '\0') {
      transmit(*s);
      s++;
    }
  }

  uint8_t receive()
  {
    /* Wait for data to be received */
    while (!dataAvailable())
    {}

    /* Get and return received data from buffer */
    return UDR0;
  }

  bool dataAvailable()
  {
    return UCSR0A & ( 1 << RXC0);
  }
}
