#include <common/led_spi.h>
#include <avr/io.h>
#include <util/delay.h>

using namespace pov;

int main()
{
  led_spi::init();
    
  while(1) {
    using namespace led_spi;
    setAllLedsUp();
    masterTransmit();
    _delay_ms(500);

    setAllLedsDown();
    masterTransmit();
    _delay_ms(500);

    // enable individual led
    setLedUp(15);
    setLedUp(0);
    masterTransmit();
    _delay_ms(500);

    // disable individual led
    setLedDown(15);
    setLedDown(0);
    masterTransmit();
    _delay_ms(500);

    // enable selected led and disable others
    leds_state = 1 << 2 | 1 << 3 | 1 << 5 | 1 << 6 ;
    masterTransmit();
    _delay_ms(500);

    // turn off all leds
    leds_state = 0;
    masterTransmit();
    _delay_ms(2000);
  }
  return 0;
}
