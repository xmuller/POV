#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <common/led_spi.h>
#include "common/serial.h"
#include "common/timer.h"
#include "common/encoder.h"

using namespace pov;

int main()
{
  led_spi::init();
  serial::init();
  sei(); 
  
  while (1) {   
    serial::transmit("no");
    EIMSK = EIMSK | (1 << INT0); // enable external interruption
    _delay_ms(1000);
    //PCMSK0  |= (1 << PCINT0);
  }
  return 0;
}
