#pragma once

#include <stdint.h>

namespace pov::encoder {
  inline constinit uint16_t time_per_round = 0;
  inline constinit uint32_t current_pov_speed = 0;
  inline constinit unsigned long int current_pov_position = 0;

  int getHallSensor();
  void init();
}
