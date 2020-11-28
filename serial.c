#include <avr/io.h>
#include <util/delay.h>

#include "common/serial.h"

int main() {
  USART_Init(MYUBRR);

  while (1) {
    USART_Transmit('x');
    USART_Transmit_String("Coucou");
    _delay_ms(1000);
  }
  return 0;
}
