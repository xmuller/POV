#pragma once

#include "serial.h"
#include "shell.h"
#include "timer.h"
#include "common/led_spi.h"

namespace pov
{
inline constinit bool warning_enabled = false;
inline bool run_shell = false;

constexpr uint16_t RADIUS_MICRO_METER = 0.09e4;
constexpr uint16_t PIx2 = 3.1415e4 * 2;

namespace led_spi {
constexpr Config config  {
  .INITIAL_LEDS_STATE = 0
};
}

// TIMER
namespace timer {
constexpr Config<2> config {
  .CONFIGURED_TIMERS {
    {
      .TIMER_ID = TimerIdentifiers::TIMER_0_8_BIT,
      .CONTROL_FLAGS = ControlFlags::PRESCALED_256,
      .ENABLED_INTERRUPTION_FLAGS = InterruptFlags::INTERRUPT_OVERFLOW
    },
    {
      .TIMER_ID = TimerIdentifiers::TIMER_1_16_BIT,
      .CONTROL_FLAGS = ControlFlags::PRESCALED_64,
      .ENABLED_INTERRUPTION_FLAGS = InterruptFlags::INTERRUPT_OVERFLOW
    },
  }
};
}

// SERIAL
namespace serial {
constexpr Config config {
  .BAUD_RATE 	 = 38400,
  .BAUD_RATE_TOL = 10,
  .MODE = serial::Mode::ASYNCHRONOUS_NORMAL,
  .PRINT_BUFFER_SIZE = 100
};
}

namespace shell {
constexpr Config config {
  .MAX_ARGUMENTS = 4,
  .MAX_LINE_SIZE = 64,
  .NEW_LINE_STRING = "\n",
  .PROMPT = "pov> "
};
}

}
