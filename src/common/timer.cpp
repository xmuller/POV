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

uint8_t getHours()
{
  return hours%12;
}

uint8_t getMinutes()
{
  return minutes;
}

uint8_t getSeconds()
{
  return (uint8_t)(overflowCounter0 / calculateTimerTicksPerSec<0>() );
}

void setHours(uint8_t value)
{
  hours = value;
}

void setMinutes(uint8_t value) {
  minutes = value;
}

void setSeconds(uint8_t value) {
  setInterruptionFlags<0>(0 << 0);
  uint8_t current_sec = getSeconds();
  overflowCounter0 = overflowCounter0 - current_sec * calculateTimerTicksPerSec<0>();
  overflowCounter0 = overflowCounter0 + value * calculateTimerTicksPerSec<0>();
  TCNT0 = 0;
  setInterruptionFlags<0>(1 << 0);
}

ISR(TIMER0_OVF_vect)
{
  overflowCounter0 = overflowCounter0 + 1;
  if( overflowCounter0 == calculateTimerTicksPerSec<0>()* 60) {
     overflowCounter0 = 0;
     minutes = minutes + 1;
     if ( minutes == 60) {
       hours = hours + 1;
       minutes = 0;
     }
  }
}

ISR(TIMER1_OVF_vect)
{
  if(warning_enabled)
    serial::transmit("Error: Timer1 shouldn't overflow ! (reset by encoder)\n");
}

}
