#include "encoder.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "common/serial.h"
#include "timer.h"
#include "led_spi.h"


namespace pov::encoder
{
ISR(INT0_vect) 
{
  current_pov_position = 0;
  time_per_round = timer::getCurrentTime<1>();
  TCNT1 = 0;
  timer::tickCounters[0] = 0;
}

void init()
{
    EIMSK = (1 << INT0);
    EICRA = (1 << ISC00) | (1 << ISC01);   //Génère une interruption à chaque fois que INT0 passe de 0 à 1
}


int getHallSensor(){
    return (PIND & _BV(PD2));
}
}
