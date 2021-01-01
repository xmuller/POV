#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <common/led_spi.h>
#include "common/serial.h"
#include "common/timer.h"
#include "common/encoder.h"


int main()
{
  SPI_init();
  USART_Init(MYUBRR);
  sei(); 
  
  while (1) {   
    USART_Transmit_String("no");
    EIMSK |= (1 << INT0);
    _delay_ms(1000);
    //PCMSK0  |= (1 << PCINT0);
  }
  return 0;
}
