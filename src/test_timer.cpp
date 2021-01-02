#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>

#include "common/serial.h"
#include "common/timer.h"

using namespace pov;
using namespace pov::serial;

int main()
{
    sei();
    serial::init();
    timer::init();
    char buf[16];
    while (1)
    {
      sprintf(buf, "seconds %hhu\n", timer::getSeconds());
      transmit(buf);
      sprintf(buf, "minutes %hhu\n", timer::getMinutes());
      transmit(buf);
      sprintf(buf, "hours %hhu\n", timer::getHours());
      transmit(buf);
      _delay_ms(1000);
    }
    return 0;
}
