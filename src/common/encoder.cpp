#include "encoder.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "common/serial.h"
#include "timer.h"
#include "led_spi.h"

#include "config.h"

namespace pov::encoder
{
ISR(INT0_vect) 
{
  current_pov_position = 0;
  time_per_round = timer::getCurrentTime<1>();
  current_pov_speed = RADIUS_MICRO_METER * PIx2 / encoder::time_per_round;
  TCNT1 = 0;
}

void init()
{
  EIMSK = (1 << INT0);
  EICRA = (1 << ISC00) | (1 << ISC01);   // external interrupt on falling edge
}

int getHallSensor(){
    return (PIND & _BV(PD2));
}
}
