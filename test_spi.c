#include "led.h"
#include <avr/io.h>
#include <util/delay.h>

void main()
{
    SPI_init();
    
    while(1) {
        // enable individual led
        SPI_Set_Led_UP(15);
        SPI_Set_Led_UP(0);
        SPI_MasterTransmit();
        _delay_ms(500);

        // disable individual led
        SPI_Set_Led_DOWN(15);
        SPI_Set_Led_DOWN(0);
        SPI_MasterTransmit();
        _delay_ms(500);
        

        // enable selected led and disable others
        leds_state = 1 << 2 | 1 << 3 | 1 << 5 | 1 << 6 ;
        SPI_MasterTransmit();
        _delay_ms(500);

        // turn off all leds
        leds_state = 0;
        SPI_MasterTransmit();
        _delay_ms(1000);
    }
}