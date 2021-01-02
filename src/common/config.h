#pragma once

#include "serial.h"
#include "timer.h"
#include "common/led_spi.h"

namespace pov
{
namespace led_spi {
constexpr Config config  {
  .INITIAL_LEDS_STATE = 0b1111111111111111
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
      .CONTROL_FLAGS = ControlFlags::PRESCALED_256,
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
  .MODE = serial::Mode::ASYNCHRONOUS_NORMAL
};
}

}
