#define __DELAY_BACKWARD_COMPATIBLE__

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "common/serial.h"
#include "common/timer.h"
#include "common/led.h"
#include "common/encoder.h"

void main()
{
    sei();
    SPI_init();
    USART_Init(MYUBRR);
    init_timer();
    init_encoder();
    while (1)
    {
        double vel = getVelocity();
        _delay_ms(100);
        char buf[16];
        sprintf(buf, "velocity %u\n", getVelocity());
        USART_Transmit_String(buf);
        //SPI_Set_ALL_Leds_DOWN();
        //SPI_MasterTransmit();
        //setBigNeedle();
    }
}
