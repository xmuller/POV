#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "common/serial.h"
#include "timer.h"
#include "led.h"

static volatile unsigned int velocity = 0;

ISR(INT0_vect) //Sans interruption, pour lire c'est (PIND & _BV(PD2))
{
    setAngle(0);
    velocity = getVelocityAndReset();
}

unsigned int getVelocity()
{
    return velocity;
}

int getHallSensor(){
    return (PIND & _BV(PD2));
}