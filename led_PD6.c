#include <avr/io.h>
#include <util/delay.h>

void main()
{
  DDRD |= _BV(PD6);
  while (1) {

    PORTD |= _BV(PD6);
    _delay_ms(500);

    PORTD &= ~_BV(PD6);
    _delay_ms(500);
  }
}
