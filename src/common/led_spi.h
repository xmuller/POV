#pragma once

#include <stdint.h>

namespace pov::led_spi {
  struct Config {
    unsigned short INITIAL_LEDS_STATE;
  };

  extern constinit unsigned short leds_state;
  extern constinit unsigned int vitesse;

  void init();

  void setLedUp(uint8_t id);
  void setLedDown(uint8_t id);

  void setAllLedsUp();
  void setAllLedsDown();

  void masterTransmit();

  void setBigNeedle();
  void setLittleNeedle();

  inline unsigned char internalLedsStatus();
  inline unsigned char externalLedsStatus();
}
