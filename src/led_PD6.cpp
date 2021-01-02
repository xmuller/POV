#include <avr/io.h>
#include <util/delay.h>

#include <common/led_spi.h>
#include "common/serial.h"
#include "common/timer.h"
#include "common/encoder.h"

using namespace pov;

int main()
{
  serial::init();
  DDRD = DDRD | _BV(PD6);
  while (1) {

    PORTD = PORTD | _BV(PD6);
    _delay_ms(500);

    PORTD = PORTD & (uint8_t)(~_BV(PD6));
    _delay_ms(500);

  }
  return 0;
}
