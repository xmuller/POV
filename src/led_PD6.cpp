#include <avr/io.h>
#include <util/delay.h>

#include <common/led_spi.h>
#include "common/serial.h"
#include "common/timer.h"
#include "common/encoder.h"

int main()
{

  USART_Init(MYUBRR);
  DDRD |= _BV(PD6);
  while (1) {

    PORTD |= _BV(PD6);
    _delay_ms(500);

    PORTD &= ~_BV(PD6);
    _delay_ms(500);

  }
  return 0;
}
