#pragma once

#include <stdint.h>
#include <avr/io.h>

namespace pov::timer
{
constexpr uint8_t NB_MAX_TIMERS = 3;

// we are using an overflow counter because the timer overflow in less than 1 sec
// this value is used to count the secondes
inline volatile uint16_t overflowCounter0 = 0;

inline volatile uint8_t hours = 0;
inline volatile uint8_t minutes = 0;

template<uint8_t TIMER_ID>
inline uint16_t getCurrentTime() {
  static_assert (TIMER_ID < NB_MAX_TIMERS, "Exceed maximum timers identifier."); \
  if constexpr (TIMER_ID == 1)
    return (TCNT1);
  if constexpr (TIMER_ID == 0)
    return TCNT0;
}

uint8_t getHours();
uint8_t getMinutes();
uint8_t getSeconds();

void setHours(uint8_t value);
void setMinutes(uint8_t value);
void setSeconds(uint8_t value);

/**
 * @brief init system_clock
 * The system_clock is configured as compile time using system_clock::config
 * defined inside config.h by default
 */
void init();

enum TimerIdentifiers : uint8_t {
  TIMER_0_8_BIT,
  TIMER_1_16_BIT,
  TIMER_2_8_BIT,
};

enum InterruptFlags : uint8_t {
  INTERRUPT_OVERFLOW      = (1 << 0),
  OUTPUT_COMPARE_B_MATCH  = (1 << 1),
  OUTPUT_COMPARE_1_MATCH  = (1 << 2),
};

/**
 * @see Table 14-8. Waveform Generation Mode Bit Description (ATmega328P DATASHEET)
 * @see Table 17-9. Clock Select Bit Description (ATmega328P DATASHEET)
 */
enum ControlFlags : uint16_t {
// Clock Source                              CSx2       CSx1       CSx0
  CLOCK_STOP                     	 = (0 << 2) | (0 << 1) | (0 << 0),
  NOT_PRESCALED                  	 = (0 << 2) | (0 << 1) | (1 << 0),
  PRESCALED_8                   	 = (0 << 2) | (1 << 1) | (0 << 0),
  PRESCALED_64                  	 = (0 << 2) | (1 << 1) | (1 << 0),
  PRESCALED_256                          = (1 << 2) | (0 << 1) | (0 << 0),
  PRESCALED_1024                         = (1 << 2) | (0 << 1) | (1 << 0),
  EXTERNAL_CLOCK_SOURCE_T1_FAILLING_EDGE = (1 << 2) | (1 << 1) | (0 << 0),
  EXTERNAL_CLOCK_SOURCE_T1_RISING_EDGE   = (1 << 2) | (1 << 1) | (1 << 0),
  FORCE_OUTPUT_COMPARE_B                 = (1 << 6),
  FORCE_OUTPUT_COMPARE_A                 = (1 << 7),
// Waveform Generation Mode                  WGM2       WGM1      WGM0
  WG_NORMAL                              = (0 << 3) | (0 << 9) | (0 << 8),
  WG_PWM_PHASE_CORRECT_MAX_TOP           = (0 << 3) | (0 << 9) | (1 << 8),
  WG_CTC                                 = (0 << 3) | (0 << 9) | (1 << 8),
  WG_FAST_PWM                            = (0 << 3) | (1 << 9) | (1 << 8),
  WG_PWM_PHASE_CORRECT_CUSTOM_TOP        = (1 << 3) | (0 << 9) | (1 << 8),
  WG_FAST_CUSTOM_TOP                     = (1 << 3) | (1 << 9) | (1 << 8),
// Compare Output Mode
};

struct TimerConfig {
  TimerIdentifiers TIMER_ID;
  uint16_t CONTROL_FLAGS;
  uint8_t ENABLED_INTERRUPTION_FLAGS;
};

template<uint8_t NB_TIMER_USED_>
struct Config
{
  static_assert (NB_TIMER_USED_ >= 0 && NB_TIMER_USED_ <= NB_MAX_TIMERS, "You exceed the number of timers available on this platform.");

  static constexpr uint8_t NB_TIMER_USED = NB_TIMER_USED_;
  const TimerConfig CONFIGURED_TIMERS[NB_TIMER_USED_];

  constexpr TimerConfig operator[](int i) const {return Config::CONFIGURED_TIMERS[i]; }
};

#define setterRegister(function_name, param_type, register0, register1, register2) \
template <uint8_t TIMER_ID> \
inline constexpr void function_name(param_type flags) { \
    static_assert (TIMER_ID < NB_MAX_TIMERS, "Exceed maximum timers identifier."); \
    if constexpr (TIMER_ID == 0) { \
        register0 = register0 | flags; \
    } \
    if constexpr (TIMER_ID == 1) { \
        register1 = register1 | flags; \
    } \
    if constexpr (TIMER_ID == 2) { \
        register2 = register2 | flags; \
    } \
}

setterRegister(setInterruptionFlags, uint8_t, TIMSK0, TIMSK1, TIMSK2)
setterRegister(setControlFlagsA, uint8_t, TCCR0A, TCCR1A, TCCR2A)
setterRegister(setControlFlagsB, uint8_t, TCCR0B, TCCR1B, TCCR2B)

template<uint8_t TIMER_ID>
inline constexpr void setControlFlags(uint16_t flags) {
  setControlFlagsA<TIMER_ID>(static_cast<uint8_t>(flags >> 8));
  setControlFlagsB<TIMER_ID>(static_cast<uint8_t>(flags >> 0));
}
#undef setterRegister
}
