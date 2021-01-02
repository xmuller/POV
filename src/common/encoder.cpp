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

ISR(INT0_vect) //Sans interruption, pour lire c'est (PIND & _BV(PD2))
{
    timer::setAngle(0);
    velocity = timer::getVelocityAndReset();
    EIMSK = EIMSK & (0 << INT0);
}

unsigned int getVelocity()
{
    return velocity;
}

int getHallSensor(){
    return (PIND & _BV(PD2));
}
}
