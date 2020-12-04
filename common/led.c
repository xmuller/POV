#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char leds_packet[2] = { 0b00000000, 0b00000000 };
unsigned char leds_packet_2[2] = { 0b11111111, 0b11111111 };

void SPI_Set_Led_UP(char id) {
    *leds_packet = (1 << id);
}

void SPI_Set_Led_DOWN(char id) {
    *leds_packet = (0 << id);
}

void SPI_init()
{
    /* Set MOSI and SCK output, all others input */
    DDRB = (1<<DDB3)|(1<<DDB5);

    DDRC = (1<<PC1) | (1<<PC2);        //Set latch and OE to 0
    
    PORTC = (0 << PC1) & (0 << PC2);

    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
} 

char SPI_SlaveReceive(void)
{
    /* Wait for reception complete */
    while(!(SPSR & (1<<SPIF)));
    
    /* Return Data Register */
    return SPDR;
}

void SPI_MasterTransmit(int i)
{
    /* Start transmission */
    if (i == 0) 
        SPDR = leds_packet[0];
    else
        SPDR = leds_packet_2[0];

    //_delay_ms(10);
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));

    if (i == 0) 
        SPDR = leds_packet[1];
    else
        SPDR = leds_packet_2[1];

    //_delay_ms(10);
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));

    PORTC = (1 << PC2);    // enable latch
    PORTC = (0 << PC2);    // disable latch (time to write > time to flush)
}