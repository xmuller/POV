#include <avr/interrupt.h>
#include <util/delay.h>

#include "timer.h"

#define S_CYCLES F_CPU
#define MS_CYCLES (F_CPU / 1000)
#define US_CYCLES (F_CPU / 1000000)

static unsigned int count = 0;

ISR(TIMER0_OVF_vect)
{
    count++;
}

//TIFR0 //Timer interrupt register
//TOV0 //Timer overflow flag
void init_timer()
{
    TIFR0 |= _BV(TOV0);
}

//TCNT0 //Timer register
static long time()
{
    return count * 256 + TCNT0;
}

unsigned short time_sec()
{
    return (short)(time() / S_CYCLES);
}

unsigned int time_milli()
{
    return (int)(time() / MS_CYCLES);
}

unsigned long time_micro()
{
    return (long)(time() / US_CYCLES);
}
