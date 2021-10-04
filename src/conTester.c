#include "conTester.h"


/* Shiftregisters */
shift74HC595 shiftOUT;
shift74HC165 shiftIN;


void ConTester_init(void (*latch595)(void), void (*latch165)(uint8_t state)) {
  shift74HC595_init(&shiftOUT,latch595,3*NUMBER_OF_BOARDS);
  shift74HC595_reset(&shiftOUT);
  shift74HC165_init(&shiftIN,latch165,3*NUMBER_OF_BOARDS);
}


void set_595_Output(uint8_t pinToSet) {
  uint8_t number_of_chips = 3*NUMBER_OF_BOARDS;
  uint8_t shiftmask[number_of_chips];
  for(int i=0;i<number_of_chips;i++) {
    shiftmask[i] = (1<<(pinToSet-(8*i)))>>1; // convert uint value to shiftmask[] array of 8bits.
  }
  shift74HC595_out(&shiftOUT,shiftmask);
}

void reset_595s() {
  uint8_t number_of_chips = 3*NUMBER_OF_BOARDS;
  uint8_t shiftmask[number_of_chips];
  for(int i=0;i<3;i++) {
    shiftmask[i] = 0x00;
  }
  shift74HC595_out(&shiftOUT,shiftmask);
}

void scan(uint8_t data[][3]) {
  uint8_t data_received[] = {0x00,0x00,0x00};
  uint8_t shiftmask[] = {0b00000001};
  
  for(int i=0; i<8; i++) {
    shiftmask[0] = (1<<i);
    shift74HC595_out(&shiftOUT,shiftmask);
    shift74HC165_read(&shiftIN,data_received);
    for(int j=0; j<3; j++) {
      data[i][j] = data_received[j];
    }
  }
}