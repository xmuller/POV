#pragma once

namespace pov::encoder {
  inline constinit unsigned long int time_per_round = 0;
  inline constinit unsigned long int current_pov_position = 0;

  int getHallSensor();
  void init();
}
