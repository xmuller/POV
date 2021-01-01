#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#include "timer.h"
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

unsigned int getHours() 
{  
    int hours = (countTimer0 / (nbCycleTimer0 * 60 * 12) ) % 60;
    return hours;   
}

unsigned int getMinutes() 
{  
    int minutes = (countTimer0 / (nbCycleTimer0 * 60) ) % 60;
    return minutes; 
}

unsigned int getSeconds()    
{   
    int seconds = (countTimer0 / nbCycleTimer0) % 60;
    return seconds; 
}

ISR(TIMER0_OVF_vect)
{
    if(etallonnage == true)
        nbCycleTimer0++;  
    else
    {
        countTimer0++;
        if(countTimer0 == nbCycleTimer0 * 3600 * 12)
            countTimer0 = 0;
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
