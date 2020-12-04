#include <avr/io.h>
#include <stdio.h>
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
        _delay_ms(1000);
        unsigned long time2 = time_micro();
        char buffer[16];
        sprintf(buffer, "%d\n", time2);
        USART_Transmit_String(buffer);
    }
}
