#include <avr/io.h>
#include <util/delay.h>

#include "common/serial.h"
using namespace pov;

int main() {
  serial::init();

  while (1) {
    serial::transmit('x');
    serial::transmit("Coucou");
    _delay_ms(1000);
  }
  return 0;
}
