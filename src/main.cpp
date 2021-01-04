#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#include "common/encoder.h"
#include "common/led_spi.h"
#include "common/serial.h"
#include "common/shell.h"
#include "common/timer.h"
#include "common/config.h"

using namespace pov;

uint8_t current_mode = 0;
uint16_t main_loop_duration = 0;

void povInfo(unsigned int, char* []) {
  serial::printf("current mode       : %hu\n"
                 "pov speed RPM      : %hu\n", current_mode, main_loop_duration);
   serial::printf("time (us) one turn : %hu\n"
                 "time main loop (us): %hu\n", main_loop_duration, main_loop_duration);
}


void time(unsigned int argc, char* argv []) {
  if (argc != 3 && argc != 0)
    serial::printf("Error: cmd require 0 or 3 arguments: hour min sec\n");

  if (argc == 3 ) {
    timer::setHours((uint8_t)atoi(argv[0]));
    timer::setMinutes((uint8_t)atoi(argv[1]));
    timer::setSeconds((uint8_t)atoi(argv[2]));
    serial::transmit("NEW ");
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

void mode(unsigned int argc, char* argv[]) {
  if (argc == 1){
    current_mode = (uint8_t)atoi(argv[0]);
    serial::transmit("NEW ");
  }
    serial::printf("Mode value is: %i\n", current_mode);
}

const shell::ShellCommand shell::shell_commands[] = {
 {"pov_info", povInfo},
 {"time", time},
 {"mode", mode},
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



#define TIME_TO_ACTIVATE 500
#define WIDTH_DIGIT 600

void loopMode1()
{
  led_spi::setAllLedsDown();
  auto current_time = timer::getCurrentTime<1>();
  char buf[12];
  sprintf(buf, "%u:%u:%u", timer::getHours(), timer::getMinutes(), timer::getSeconds());
  for(unsigned int i = 0; i < 12; i++)
  {
    if(buf[i] == '0')
    {
      if(current_time > (0.15 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.85 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT))
      {
        led_spi::setLedUp(12);
        led_spi::setLedUp(6);
      }

      if((current_time > (0.05 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.25 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) )
        || (current_time > (0.75 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.95 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT)))
      {
        led_spi::setLedUp(11);
        led_spi::setLedUp(7);
      }

      if((current_time > (i * WIDTH_DIGIT) && current_time < (0.1 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) )
      || (current_time > (0.9 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (TIME_TO_ACTIVATE + i * WIDTH_DIGIT) ))
      {
        led_spi::setLedUp(10);
        led_spi::setLedUp(9);
        led_spi::setLedUp(8);
      }
    }
    if(buf[i] == '1')
    {
      if(current_time > (0.4 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.6 * TIME_TO_ACTIVATE + i * 300))
      {
        led_spi::setLedUp(12);
        led_spi::setLedUp(10);
        led_spi::setLedUp(9);
        led_spi::setLedUp(8);
        led_spi::setLedUp(7);
      }

      if(current_time > (0.35 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.65 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(11);

      if(current_time > (0.3 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.7 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(6);
    }
    if(buf[i] == '2')
    {
      if(current_time > (0.25 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.85 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(12);

      if((current_time > (0.05 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.25 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) )
        || (current_time > (0.7 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.95 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT)))     
        led_spi::setLedUp(11);
      
      if(current_time > (0.55 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.7 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(10);

      if(current_time > (0.4 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.55 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(9);

      if(current_time > (0.25 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.4 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(8);

      if(current_time > (0.1 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.25 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(7);

      if(current_time > (0.05 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.95 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(6);

    }
    if(buf[i] == '3')
    {
      if(current_time > (0.0 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.8 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(12);
        
      if(current_time > (0.6 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.75 * TIME_TO_ACTIVATE + i * 300))
      {
        led_spi::setLedUp(11);
        led_spi::setLedUp(9);
      }

      if(current_time > (0.45 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.6 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(10);
      
      if(current_time > (0.75 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.9 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(8);
      
      if((current_time > (0.0 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.2 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) )
        || (current_time > (0.8 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.95 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT)))     
        led_spi::setLedUp(7);

      if(current_time > (0.2 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.8 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(6);

    }
    if(buf[i] == '4')
    {
      if(current_time > (0.5 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.8 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(12);

      if((current_time > (0.4 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.55 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) )
        || (current_time > (0.6 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.8 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT)))     
        led_spi::setLedUp(11);

      if((current_time > (0.3 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.45 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) )
        || (current_time > (0.6 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.8 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT)))     
        led_spi::setLedUp(10);

      if((current_time > (0.15 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.35 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) )
        || (current_time > (0.6 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.8 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT)))     
        led_spi::setLedUp(9);
      
      if(current_time > (0.0 * TIME_TO_ACTIVATE + i * 300) && current_time < (1.0 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(8);

      if(current_time > (0.6 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.8 * TIME_TO_ACTIVATE + i * 300))
      {
        led_spi::setLedUp(7);
        led_spi::setLedUp(6);
      }
    }
    if(buf[i] == '5')
    {
      if(current_time > (i * 300) && current_time < (0.85 * TIME_TO_ACTIVATE + i * 300))
      { 
        led_spi::setLedUp(12);
        led_spi::setLedUp(10);
      }

      if(current_time > (i * 300) && current_time < (0.1 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(11);

      if(current_time > (0.75 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.9 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(9);

      if(current_time > (0.8 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.95 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(8);

      if((current_time > (i * WIDTH_DIGIT) && current_time < (0.15 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) )
        || (current_time > (0.75 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.9 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT)))     
        led_spi::setLedUp(7);

      if(current_time > (0.15 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.85 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(6);
    }
    if(buf[i] == '6')
    {
      if(current_time > (0.15 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.85 * TIME_TO_ACTIVATE + i * 300))
      {
        led_spi::setLedUp(12);
        led_spi::setLedUp(10);
        led_spi::setLedUp(6);
      }

      if(current_time > (0.05 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.25 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(11);

      if((current_time > (0.05 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.25 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) )
        || (current_time > (0.75 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.95 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT)))     
      {
        led_spi::setLedUp(9);
        led_spi::setLedUp(7);
      }

      if((current_time > (i * 300) && current_time < (0.1 * TIME_TO_ACTIVATE + i * 300))
        || (current_time > (0.9 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (TIME_TO_ACTIVATE + i * WIDTH_DIGIT)))     
        led_spi::setLedUp(8);
    }
    if(buf[i] == '7')
    {
      if(current_time > (0.1 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.9 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(12);

      if(current_time > (0.75 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.9 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(11);
      
      if(current_time > (0.6 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.75 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(10);

      if(current_time > (0.45 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.6 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(9);

      if(current_time > (0.3 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.45 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(8);

      if(current_time > (0.15 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.3 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(7);

      if(current_time > (i * 300) && current_time < (0.15 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(6);
    }
    if(buf[i] == '8')
    {
      if(current_time > (0.15 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.85 * TIME_TO_ACTIVATE + i * 300))
      {  
        led_spi::setLedUp(12);
        led_spi::setLedUp(6);
      }

      if((current_time > (0.05 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.25 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) )
        || (current_time > (0.75 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.95 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT)))     
      {
        led_spi::setLedUp(11);
        led_spi::setLedUp(7);
        led_spi::setLedUp(8);
      }

      if((current_time > (i * 300) && current_time < (0.9 * TIME_TO_ACTIVATE + i * 300))
        || (current_time > (0.9 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (TIME_TO_ACTIVATE + i * WIDTH_DIGIT)))     
      {
        led_spi::setLedUp(10);
        led_spi::setLedUp(9);
      }
    }
    if(buf[i] == '9')
    {
      if(current_time > (0.15 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.85 * TIME_TO_ACTIVATE + i * 300))
      {  
        led_spi::setLedUp(12);
        led_spi::setLedUp(8);
        led_spi::setLedUp(6);
      }

      if((current_time > (0.05 * TIME_TO_ACTIVATE + i * 300) && current_time < (0.25 * TIME_TO_ACTIVATE + i * 300))
        || (current_time > (0.75 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.95 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT)))     
      {
        led_spi::setLedUp(11);
        led_spi::setLedUp(9);
      }

      if(current_time > (i * 300) && current_time < (0.9 * TIME_TO_ACTIVATE + i * 300))
        led_spi::setLedUp(10);
    }
    if(buf[i] == ':')
    {
      if(current_time > (0.45 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT) && current_time < (0.55 * TIME_TO_ACTIVATE + i * WIDTH_DIGIT))
      {
        led_spi::setLedUp(9);
        led_spi::setLedUp(6);
      }
    }
  }

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
    main_loop_duration = timer::getCurrentTime<1>();
    if(current_mode == 0)
      loopMode0();
    if(current_mode == 1)
      loopMode1();
    if(current_mode == 4) // idle mode
      _delay_ms(300);
    main_loop_duration = timer::getCurrentTime<1>() - main_loop_duration;

    if(run_shell)
      shell::shellTick();
  }
  return 0;
}
