#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "common/serial.h"

ISR(INT0_vect) //Sans interruption, pour lire c'est (PIND & _BV(PD2))
{
   USART_Transmit_String("Coucou");
   _delay_ms(1000);
   EIMSK &= (0 << INT0);
}


int getHallSensor(){
    return (PIND & _BV(PD2));
}