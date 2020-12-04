#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#include "common/serial.h"
#include "common/timer.h"

void main()
{
    USART_Init(MYUBRR);
    init_timer();

    while (1)
    {
        unsigned long time1 = time_micro();
        _delay_ms(500);
        unsigned long time2 = time_micro();
        unsigned char buffer[16];
        USART_Transmit_String(itoa(time2, buffer, 10));
    }
}
