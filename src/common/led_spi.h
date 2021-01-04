#pragma once

#include <stdint.h>

namespace pov::led_spi {
  struct Config {
    uint16_t INITIAL_LEDS_STATE;
  };

  extern uint16_t leds_state;

  void init();

  void setLedUp(uint8_t id);
  void setLedDown(uint8_t id);

  void setAllLedsUp();
  void setAllLedsDown();

  void masterTransmit();

  inline uint8_t internalLedsStatus();
  inline uint8_t externalLedsStatus();
}
