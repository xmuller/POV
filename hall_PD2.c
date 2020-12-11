#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "led.h"
#include "common/serial.h"
#include "common/timer.h"
#include "common/encoder.h"


void main()
{
    USART_Init(MYUBRR);
    sei(); 
    PCMSK0  |= (1 << PCINT0);
  while (1) {
       USART_Transmit('0');

      
  }
}
