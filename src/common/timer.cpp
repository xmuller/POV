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
template<int N>
void configAllTimers() {
  setControlFlags<config[N-1].TIMER_ID>(config[N-1].CONTROL_FLAGS);
  setInterruptionFlags<config[N-1].TIMER_ID>(config[N-1].ENABLED_INTERRUPTION_FLAGS);

  if constexpr (N > 1)
    configAllTimers<N-1>();
}

void init() {
  configAllTimers<config.NB_TIMER_USED>();

  etallonnage = true;
  _delay_ms(1000);
  etallonnage = false;
}

unsigned int getVelocityAndReset()  //À appeler chaque fois que l'on passe sur l'aimant
{
    unsigned int velocity = (1 << 12) * countTimer1 + TCNT1;
    TCNT1 = 0;
    countTimer1 = 0;
    return velocity;
}

double getAngle()   {   return angle;   }
double getAngleMinute()   {   return angleMinute;   }
double getAngleHour()   {   return angleHour;   }
void setAngle(double ang)   {   angle = ang;    }

unsigned int getHours() {  return hours;   }
unsigned int getMinutes() {  return minutes; }
unsigned int getSeconds()    {   return seconds; }

ISR(TIMER0_OVF_vect)
{
    if(etallonnage == true)
        nbCycleTimer0 = nbCycleTimer0 + 1;
    else
    {
        countTimer0 = countTimer0 + 1;
        if(countTimer0 == nbCycleTimer0)
        {
            countTimer0 = 0;
            seconds = seconds + 1;
            if(seconds == 60)
            {
                seconds = 0;
                minutes = minutes + 1;
                angleMinute = angleMinute + 2 * M_PI / 60;
                if (minutes == 60)
                {
                    angleMinute = 0;
                    minutes = 0;
                    hours = hours + 1;
                    angleHour = angleHour + 2 * M_PI / 12;
                    if (hours == 24)
                    {
                        angleHour = 0;
                        hours = 0;
                    }
                }
            }
        }
    }  
}

ISR(TIMER1_OVF_vect)
{
    countTimer1 = countTimer1 + 1;
}

}
