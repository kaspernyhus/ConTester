/*
---------------------------------
SPI module for
ARDUINO NANO
---------------------------------

MISO  =>  PB4
MOSI  =>  PB3 
SCK   =>  PB5

*/  

#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


#define MOSI    PB3
#define MISO    PB4
#define SCK     PB5
#define SS      PB2

#define MSB_TRANSMITTED_FIRST 0
#define LSB_TRANSMITTED_FIRST 1


void SPI_master_init();
void SPI_masterTransmit(uint8_t data);
uint8_t SPI_masterReceive();
void SPI_set_data_order(uint8_t transmitted_first);

#endif /* SPI_H_ */