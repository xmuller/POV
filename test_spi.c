#include "led.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void main()
{
    SPI_init();
    SPI_MasterTransmit();
    SPI_SlaveReceive();
}