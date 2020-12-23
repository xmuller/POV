#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <stdio.h>

#include "timer.h"
#include "serial.h"

#define S_CYCLES F_CPU
#define MS_CYCLES (F_CPU / 1000)
#define US_CYCLES (F_CPU / 1000000)

volatile static unsigned int count = 0;   //Nombre de cycle 
volatile static unsigned int nbCycle = 0; //Nombre de cycle nécessaire pour faire 1 s

volatile static unsigned int seconds = 0;
volatile static unsigned int minutes = 0;
volatile static unsigned int hours = 0;

bool etallonnage = false;

int getHours() {  return hours;   }
int getMinutes() {  return minutes; }
int getSeconds()    {   return seconds; }

//#define FUSE_CKOUT

ISR(TIMER0_OVF_vect)
{
    if(etallonnage == true){
        nbCycle++;
    }
    else
    {
        count++;
        if(count == nbCycle * 256 * (2^8 - 8))
        {
            count = 0;
            seconds++;
            if(seconds == 60)
            {
                seconds = 0;
                minutes++;
                if (minutes == 60)
                {
                    minutes = 0;
                    hours++;
                    if (hours == 24)
                    {
                        hours = 0;
                    }
                    
                }
                
            }
        }
    }  
}



void init_timer()
{
    sei();
    SREG |= (1 << 7);
    
    //CLKPR |= FUSE_CKOUT;
    TIMSK0 |= _BV(TOIE0);       //Allow clock0 interrupt
    TCCR0B |= _BV(CS00);
    
    /*CLKPR |= _BV(CLKPCE);       //Clock Prescaler Change Enable
    CLKPR |= _BV(CLKPS2);      //Clock Division Factor : 256
    //CS02 mets le prescaler à 256
    TCCR0B |= (1 << CS02 && 0 << WGM02);        //bits WGMX permettent de définir le mode de
    TCCR0A |= (0 << WGM00 && 0 << WGM01);       //comptage (tous à 0 = mode normal) */
    char buf[16];
    etallonnage = true;
    _delay_ms(1000);
    etallonnage = false;    
}

//TCNT0 //Timer register
static inline long time()
{
    return count * 256 + TCNT0;
}

unsigned short time_sec()
{
    return (unsigned short)(time() / S_CYCLES);
}

unsigned int time_milli()
{
    return (unsigned int)(time() / MS_CYCLES);
}

unsigned long time_micro()
{
    return (unsigned long)(time() / US_CYCLES);
}