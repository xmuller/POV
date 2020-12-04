#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define FREQUENCE DF_CPU

void SPI_init()
{
    /* Set MOSI and SCK output, all others input */
    DDRB = (1<<DDB3)|(1<<DDB5);

    DDRC |= _BV(PC1) | _BV(PC2);        //Set latch and OE to 0
    
    PORTC &= ~_BV(PC1);
    PORTC &= ~_BV(PC2);

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

void SPI_MasterTransmit()
{
    /* Start transmission */
    unsigned char buffer[2] = { 0b11111111, 0b11111111 };
    SPDR = buffer[0];
    //_delay_ms(10);
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));

    SPDR = buffer[1];
    //_delay_ms(10);
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));

    PORTC |= _BV(PC2);      //latch
    PORTC &= ~_BV(PC2);

}