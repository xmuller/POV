#include "timer.h"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "serial.h"

#include "config.h"

namespace pov::timer
{
#define strcat_(x, y) x ## y
#define strcat(x, y) strcat_(x, y)
#define PRINT_CONSTEXPR(x) template <auto> struct strcat(strcat(value_of_, x), _is); static_assert(strcat(strcat(value_of_, x), _is)<x>::x, "");

template<uint8_t TIMER_ID>
consteval uint16_t calculateTimerTicksPerSec() {
  static_assert (TIMER_ID < NB_MAX_TIMERS, "Exceed maximum timers identifier.");
  uint16_t timer_register_max_value = 255;
  uint16_t prescaler_value = 0;
  if constexpr (TIMER_ID == 1) {
      timer_register_max_value = 0b1111111111111111;
  }
  if constexpr (config[TIMER_ID].CONTROL_FLAGS == ControlFlags::PRESCALED_8)
    prescaler_value = (1 << 3);
  else if constexpr (config[TIMER_ID].CONTROL_FLAGS == ControlFlags::PRESCALED_64)
    prescaler_value = (1 << 6);
  else if constexpr (config[TIMER_ID].CONTROL_FLAGS == ControlFlags::PRESCALED_256)
    prescaler_value = (1 << 8);
  else if constexpr (config[TIMER_ID].CONTROL_FLAGS == ControlFlags::PRESCALED_1024)
    prescaler_value = (1 << 10);

  return (uint16_t)(F_CPU / prescaler_value / timer_register_max_value);
}

template<int N>
void configAllTimers() {
  setControlFlags<config[N-1].TIMER_ID>(config[N-1].CONTROL_FLAGS);
  setInterruptionFlags<config[N-1].TIMER_ID>(config[N-1].ENABLED_INTERRUPTION_FLAGS);

  if constexpr (N > 1)
    configAllTimers<N-1>();
}

void init() {
  configAllTimers<config.NB_TIMER_USED>();
}

double getAngle()   {   return angle;   }
double getAngleMinute()   {   return angleMinute;   }
double getAngleHour()   {   return angleHour;   }
void setAngle(double ang)   {   angle = ang;    }

uint8_t getHours()
{
//    return (overflowCounter0 / (nbCycleTimer0 * 60 * 12) ) % 12;
    return (uint8_t)(overflowCounter0 / (calculateTimerTicksPerSec<0>()) / 3600) % 12;
}

uint8_t getMinutes()
{
//    return (uint16_t)(overflowCounter0 / (nbCycleTimer0 * 60) ) % 60;
    return (uint8_t)(overflowCounter0 / (calculateTimerTicksPerSec<0>()* 60) ) % 60;
}

uint8_t getSeconds()
{
//    return (uint16_t)(overflowCounter0 / nbCycleTimer0) % 60;
    return (uint8_t)(overflowCounter0 / calculateTimerTicksPerSec<0>() )% 60;
}

ISR(TIMER0_OVF_vect)
{
  // temps_max_overflow = 255 / calculateTimerTicksPerSec<0>();
  // temps_max_overflow = 255 / calculateTimerTicksPerSec<0>();
//  if (etallo)
//    nbCycleTimer0 = nbCycleTimer0 + 1;
//  else {
    overflowCounter0 = overflowCounter0 + 1;
//    if(overflowCounter0 == calculateTimerTicksPerSec<0>()* 3600 * 12)
//      overflowCounter0 = 0;
//  }

//  tickCounters[0] = tickCounters[0] + 255;
//  if(tickCounters[0] == calculateTimerTicksPerSec<0>() * 3600 * 12)
//    tickCounters[0] = 0;
}

ISR(TIMER1_OVF_vect)
{
  serial::transmit("Error: Timer1 shouldn't overflow ! (reset by encoder)\n");
//    countTimer1 = countTimer1 + ( 1 << 15);
}

}
