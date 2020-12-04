#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned short leds_state = 0b1111111111111111;

void SPI_Set_Led_UP(char id) {
    leds_state |= (1 << id);
}

void SPI_Set_Led_DOWN(char id) {
    leds_state &= (0 << id);
}

void SPI_init()
{
    /* Set MOSI and SCK output, all others input */
    DDRB |= (1<<DDB3)|(1<<DDB5)|(1<<PB2); // MOSI, SCK, SS

    DDRC |= (1<<PC1) | (1<<PC2);        //Set latch and OE to 0
    
    PORTC &= (0 << PC1) & (0 << PC2);

    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
} 

void SPI_MasterTransmit()
{
    #define INTERNAL_LEDS &leds_state
    #define EXTERNAL_LEDS &leds_state+8

    /* Start transmission */
    SPDR = (char)EXTERNAL_LEDS;

    //_delay_ms(10);
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));

    SPDR = (char)INTERNAL_LEDS;

    #undef INTERNAL_LEDS
    #undef EXTERNAL_LEDS

    //_delay_ms(10);
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));

    PORTC |= (1 << PC2);    // enable latch
    PORTC &= (0 << PC2);    // disable latch (time to write > time to flush)
}

void SPI_TurnOnAllLeds() {
    leds_state = 0b1111111111111111;
    SPI_MasterTransmit();
}

void SPI_TurnOffAllLeds() {
    leds_state = 0;
    SPI_MasterTransmit();
}