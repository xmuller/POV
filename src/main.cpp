#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "common/encoder.h"
#include "common/led_spi.h"
#include "common/serial.h"
#include "common/shell.h"
#include "common/timer.h"
#include "common/config.h"

using namespace pov;

constinit uint8_t current_mode = 0;

void povInfo(unsigned int, char* []) {
  serial::printf("current mode       : %i\n"
                 "pov speed RPM      : %i"
                 "time (us) one turn : %i"
                 "time main loop (us): %i", current_mode, 1000, 50000, 25550);
}


void time(unsigned int argc, char* argv []) {
  if (argc != 3 && argc != 0)
    serial::printf("Error: cmd require 0 or 3 arguments: hour min sec\n");

  if (argc == 3 ) {
    timer::setHours((uint8_t)atoi(argv[0]));
    timer::setMinutes((uint8_t)atoi(argv[1]));
    timer::setSeconds((uint8_t)atoi(argv[2]));
    serial::transmit("New ");
  }

  serial::printf("Time: hour:%hu min:%hu sec:%hu\n",
                  timer::getHours(),
                  timer::getMinutes(),
                  timer::getSeconds());
}

void blinkPD6(unsigned int argc, char* argv[]) {
  int blink_counter = (argc != 0) ? atoi(argv[0]) : 5;
  serial::printf("Blink %i times\n", blink_counter);

  DDRD = DDRD | (1 << PD6);
  while (blink_counter > 0) {
    PORTD = PORTD | (1 << PD6);
    _delay_ms(500);
    PORTD = PORTD & (0 << PD6);
    _delay_ms(500);
    blink_counter--;
  }
}

void toggleWarning(unsigned int, char*[]) {
  warning_enabled = !warning_enabled;
  serial::printf("Warning status: %i\n", warning_enabled);
}

void testHall(unsigned int argc, char* argv[]) {
  int test_duration_sec = (argc != 0) ? atoi(argv[0]) : 3;

  uint8_t begin_time = timer::getSeconds();
  uint16_t end_time = (uint8_t)(begin_time + test_duration_sec)%60;
  bool end = false;
  while(!end) {
    uint8_t time = timer::getSeconds();
    end = (time == end_time);
    serial::printf("Hall sensor: %i    time:(%i/%i) \n", encoder::getHallSensor(), time, end_time);
    _delay_ms(500);
  }
}

void testSpi(unsigned int, char* []) {
  using namespace led_spi;
  setAllLedsUp();
  masterTransmit();
  _delay_ms(500);

  setAllLedsDown();
  masterTransmit();
  _delay_ms(500);

  // enable individual led
  setLedUp(15);
  setLedUp(0);
  masterTransmit();
  _delay_ms(500);

  // disable individual led
  setLedDown(15);
  setLedDown(0);
  masterTransmit();
  _delay_ms(500);

  // enable selected led and disable others
  leds_state = 1 << 2 | 1 << 3 | 1 << 5 | 1 << 6 ;
  masterTransmit();
  _delay_ms(500);

  // turn off all leds
  leds_state = 0;
  masterTransmit();
  _delay_ms(500);
}

const shell::ShellCommand shell::shell_commands[] = {
 {"pov_info", povInfo},
 {"time", time},
 {"toggle_warn", toggleWarning},
 {"test_pd6", blinkPD6},
 {"test_spi", testSpi},
 {"test_hall", testHall },
 {nullptr, nullptr }
};


// Standard clock with needle
void loopMode0() {
  uint16_t time_hours_activation = encoder::time_per_round - encoder::time_per_round / 12 * (timer::getHours());
  uint16_t time_minutes_activation = encoder::time_per_round - encoder::time_per_round / 60 * (timer::getMinutes());
  uint16_t time_secondes_activation = encoder::time_per_round - encoder::time_per_round / 60 * timer::getSeconds();
  auto current_time = timer::getCurrentTime<1>();
  bool activate_hours =  current_time > time_hours_activation - 40 && current_time < time_hours_activation + 40;
  bool activate_min = current_time > time_minutes_activation - 20 && current_time < time_minutes_activation + 20;
  bool activate_sec = current_time > time_secondes_activation - 10 && current_time < time_secondes_activation + 10;

  uint16_t time_sec_bis_activation = (current_time > encoder::time_per_round/2) ? (time_secondes_activation + encoder::time_per_round/2)
                                                                                : (time_secondes_activation - encoder::time_per_round/2);
  bool activate_sec_bis = current_time >  time_sec_bis_activation - 10 && current_time < time_sec_bis_activation + 10;
  if (activate_min || activate_hours || activate_sec || activate_sec_bis) {
    if (activate_hours)
      led_spi::leds_state = 0b1111111111;
    if (activate_min)
      led_spi::leds_state = 0b1111111111111;
    if (activate_sec)
      led_spi::leds_state = 0b1111111111111;
    if (activate_sec_bis)
      led_spi::leds_state = 0b11111;
  }
  else {
    led_spi::setAllLedsDown();
  }
  led_spi::setLedUp(15);
  led_spi::masterTransmit();
}

int main() {
  sei();
  encoder::init();
  led_spi::init();
  shell::init();
  timer::init();

  while (1)
  {
    if(run_shell)
      shell::shellTick();

    if(current_mode == 0)
      loopMode0();

    if(current_mode == 4) // idle mode
      _delay_ms(500);
  }

  return 0;
}
