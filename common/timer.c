#include <avr/interrupt.h>
#include <util/delay.h>

#include "timer.h"

#define S_CYCLES F_CPU
#define MS_CYCLES (F_CPU / 1000)
#define US_CYCLES (F_CPU / 1000000)

volatile static unsigned int count = 0;

ISR(TIM0_OVF_vect)
{
    count++;
}

void init_timer()
{
    SREG |= (1 << 7);
    TIMSK0 |= _BV(TOIE0);
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
