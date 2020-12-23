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
        /*_delay_ms(1000);
        char buffer[16];
        unsigned long time1 = time_micro();
        
        unsigned long time2 = time_micro();
        sprintf(buffer, "t1 %d\n", time1);
        USART_Transmit_String(buffer);
        sprintf(buffer, "t2 %d\n", time2);
        USART_Transmit_String(buffer);
        sprintf(buffer, "dif %d\n", time2-time1);
        USART_Transmit_String(buffer);*/
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
