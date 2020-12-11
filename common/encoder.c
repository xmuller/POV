#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "common/serial.h"

ISR(PCINT0_vect) //Sans interruption, pour lire c'est (PIND & _BV(PD2))
{
   USART_Transmit_String("Coucou");
   
}


int getHallSensor(){
    return PD2;
}