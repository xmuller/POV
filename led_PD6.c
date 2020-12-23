#include <avr/io.h>
#include <util/delay.h>

#include "led.h"
#include "common/serial.h"
#include "common/timer.h"
#include "common/encoder.h"


void main()
{

  USART_Init(MYUBRR);
  DDRD |= _BV(PD6);
  while (1) {

    PORTD |= _BV(PD6);
    _delay_ms(500);

    PORTD &= ~_BV(PD6);
    _delay_ms(500);

  }
}
