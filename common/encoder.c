#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "common/serial.h"
#include "led.h"

ISR(INT0_vect) //Sans interruption, pour lire c'est (PIND & _BV(PD2))
{
   SPI_Set_ALL_Leds_UP();
   SPI_MasterTransmit();
   _delay_ms(50);
   SPI_Set_ALL_Leds_DOWN();
   SPI_MasterTransmit();
   EIMSK &= (0 << INT0);

}


int getHallSensor(){
    return (PIND & _BV(PD2));
}