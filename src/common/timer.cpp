#include "timer.h"

#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "serial.h"


unsigned int countTimer0 = 0;        //Nombre de cycle en cours sur le timer0 
unsigned long int nbCycleTimer0 = 0; //Nombre de cycle nécessaire pour faire 1s

unsigned int countTimer1 = 0;        //Nombre de cycle en cours sur le timer1 

unsigned int getVelocityAndReset()  //À appeler chaque fois que l'on passe sur l'aimant
{   
    unsigned int velocity = 2^16 * countTimer1 + TCNT1; 
    TCNT1 = 0;
    countTimer1 = 0;
    return velocity;   
}

unsigned int seconds = 0;
unsigned int minutes = 0;
unsigned int hours = 0;

bool etallonnage = false;

double angle = 0;
double angleHour = 0;
double angleMinute = 0;


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
        nbCycleTimer0++;  
    else
    {
        countTimer0++;
        if(countTimer0 == nbCycleTimer0)
        {
            countTimer0 = 0;
            seconds++;
            if(seconds == 60)
            {
                seconds = 0;
                minutes++;
                angleMinute += 2 * M_PI / 60;
                if (minutes == 60)
                {
                    angleMinute = 0;
                    minutes = 0;
                    hours++;
                    angleHour += 2 * M_PI / 12;
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
    countTimer1++;    
}

void init_timer()
{
    sei();
    SREG |= (1 << 7);
    
    TIMSK0 |= (1 << TOIE0);       //Allow clock0 interrupt
    TCCR0B |= (1 << CS02);        //Set clock0 prescaler to 256

    TIMSK1 |= (1 << TOIE1);       //Allow clock1 interrupt
    TCCR1B |= (1 << CS12);        //Set clock1 prescaler to 256
    
    etallonnage = true;
    _delay_ms(1000);
    etallonnage = false;  
}
