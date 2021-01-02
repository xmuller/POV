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
    unsigned int velocity = (1 << 12)  + TCNT1;
    TCNT1 = 0;
    countTimer1 = 0;
    return velocity;
}

double getAngle()   {   return angle;   }
double getAngleMinute()   {   return angleMinute;   }
double getAngleHour()   {   return angleHour;   }
void setAngle(double ang)   {   angle = ang;    }


unsigned long int getHours() 
{  
    hours = (countTimer0 / (nbCycleTimer0 * 60 * 12) ) % 60;
    return hours;   
}

unsigned long int getMinutes() 
{  
    minutes = (countTimer0 / (nbCycleTimer0 * 60) ) % 60;
    return minutes; 
}

unsigned long int getSeconds()    
{   
    seconds = (countTimer0 / nbCycleTimer0) % 60;
    return seconds; 
}

ISR(TIMER0_OVF_vect)
{
    if(etallonnage == true)
        nbCycleTimer0 = nbCycleTimer0 + 1;
    else
    {
        countTimer0 = countTimer0 + 1;
        if(countTimer0 == nbCycleTimer0 * 3600 * 12)
            countTimer0 = 0;
    }  
}

ISR(TIMER1_OVF_vect)
{
    countTimer1 = countTimer1 + 1;
}

}