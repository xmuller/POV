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
        char buf[16];
        sprintf(buf, "seconds %d\n", getSeconds());
        USART_Transmit_String(buf);
        sprintf(buf, "minutes %d\n", getMinutes());
        USART_Transmit_String(buf);
        sprintf(buf, "hours %d\n", getHours());
        USART_Transmit_String(buf);
        _delay_ms(1000);
    }
}
