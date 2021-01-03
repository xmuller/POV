#include "led_spi.h"
#include "encoder.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "config.h"

namespace pov::led_spi
{
  uint16_t leds_state = config.INITIAL_LEDS_STATE;

  void init()
  {
    /* Set MOSI and SCK output, all others input */
    DDRB = DDRB | (1<<DDB3)|(1<<DDB5)|(1<<PB2); // MOSI, SCK, SS

    DDRC = DDRC | (1<<PC1) | (1<<PC2);        //Set latch and OE to 0

    PORTC = PORTC & (0 << PC1) & (1 << PC2);

    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = SPCR | (1<<SPE) | (1<<MSTR);

//    SPCR = SPCR | (0<<SPI2X) | (1 << SPR1) | (1 << SPR0);
    SPCR = SPCR | (0 << SPR1) | (1 << SPR0);

    SPSR = SPSR | (1 << SPI2X);
  }

  void setLedUp(uint8_t id) {
    leds_state = leds_state | (uint16_t)(1 << id);
  }

  void setLedDown(uint8_t id) {
    leds_state =  leds_state &  (uint16_t)(0 << id);
  }

  void masterTransmit()
  {
    /* Start transmission */
    SPDR = externalLedsStatus();

    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));

    SPDR = internalLedsStatus();

    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));

    PORTC = PORTC | (1 << PC2);    // enable latch
    PORTC = PORTC & (0 << PC2);    // disable latch (time to write > time to flush)

//    _delay_us(12)
  }

  void setAllLedsUp() {
    leds_state = 0b1111111111111111;
  }

  void setAllLedsDown() {
    leds_state = 0;
  }

  uint8_t internalLedsStatus()
  {
     return static_cast<uint8_t>(leds_state >> 0);
  }

  uint8_t externalLedsStatus()
  {
     return static_cast<uint8_t>(leds_state >> 8);
  }

  void setBigNeedle()
  {
    for(uint8_t i = 0; i < 16; i++)
    {
      setLedUp(i);
      masterTransmit();
      _delay_us((double)encoder::time_per_round);
      setLedDown(i);
      masterTransmit();
    }
  }

  void setLittleNeedle()
  {
    for(uint8_t i = 0; i < 10; i++)
    {
      setLedUp(i);
      masterTransmit();
      _delay_us((double)encoder::time_per_round);
      setLedDown(i);
      masterTransmit();
    }
  }
}
