/*
---------------------------------
SPI module for
ARDUINO NANO
---------------------------------

MOSI  =>  PB3 
SCK   =>  PB5

*/

#include "SPI.h"


void SPI_master_init() {
  DDRB |= (1 << MOSI) | (1 << SCK) | (1<< SS);
  SPCR = (1 << SPE) | (1 << MSTR) | (1<<SPR1) | (1<<CPHA) | (1<<CPOL);
}

void SPI_set_data_order(uint8_t transmitted_first) {
  if(transmitted_first) SPCR |= (1 << DORD);
  else SPCR &= ~(1 << DORD);
}

void SPI_masterTransmit(uint8_t data) {
  SPDR = data;
  while(!(SPSR & (1 << SPIF))); // Wait for transmission complete
}

uint8_t SPI_masterReceive() {
  SPDR = 0xFF;
  while(!(SPSR & (1 << SPIF))); // Wait for transmission complete
  return SPDR;
}