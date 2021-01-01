#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "common/serial.h"
#include "timer.h"
#include "led.h"

unsigned int velocity = 0;

ISR(INT0_vect) 
{
    setAngle(0);
    velocity = getVelocityAndReset();
    char buf[16];
    sprintf(buf, "vel %u\n", velocity);
    USART_Transmit_String(buf);
}

void init_encoder()
{
    EIMSK |= (1 << INT0);
    EICRA |= (1 << ISC00) | (1 << ISC01);   //Génère une interruption à chaque fois que INT0 passe de 0 à 1
}

unsigned int getVelocity()
{
    return velocity;
}

int getHallSensor(){
    return (PIND & _BV(PD2));
}