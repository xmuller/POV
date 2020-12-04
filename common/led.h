#pragma once

extern unsigned short leds_state;

void SPI_init();
void SPI_MasterTransmit();

void SPI_Set_Led_UP(char id);
void SPI_Set_Led_DOWN(char id);

void SPI_Set_ALL_Leds_UP();
void SPI_Set_ALL_Leds_DOWN();

//inline void SPI_Disable_Led() {

//    PORTC |= (1 << PC1);
//}
//inline void SPI_Enable_Led() {
//    PORTC |= (1 << PC1);
//}