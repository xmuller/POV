#include "led.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "common/serial.h"
#include "stdlib.h"

char str[20];

void main()
{
    // USART_Init(MYUBRR);

    SPI_init();
    // SPI_S laveReceive();
    while(1) {
        // SPI_Set_Led_UP(3);
        SPI_MasterTransmit(1);
        _delay_ms(500);

        // SPI_Set_Led_DOWN(3);
        SPI_MasterTransmit(0);
        _delay_ms(500);
    }
}