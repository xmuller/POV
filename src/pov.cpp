#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include <common/serial.h>
#include <common/timer.h>
#include <common/led_spi.h>
#include <common/encoder.h>

using namespace pov;

int main()
{
    led_spi::init();
    serial::init();
    timer::init();

    EIMSK = EIMSK | (1 << INT0); // enable external interrupt
    sei();
    char buf[16];
    while (1)
    {
        EIMSK = EIMSK | (1 << INT0);
        _delay_ms(100);
        sprintf(buf, "velocity %u\n", encoder::velocity);
        serial::transmit(buf);
        //SPI_Set_ALL_Leds_DOWN();
        //SPI_MasterTransmit();
        //setBigNeedle();
    }
    return 0;
}
