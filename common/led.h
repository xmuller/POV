//#include "led.c"

extern unsigned char leds_packet[2];

void SPI_Set_Led_UP(char id);
void SPI_Set_Led_DOWN(char id);

void SPI_init();
void SPI_MasterTransmit(int i);
char SPI_SlaveReceive();