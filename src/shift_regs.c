#include "shift_regs.h"


/* ------------------------------- */
/*             74HC595             */
/* ------------------------------- */
void shift74HC595_init(shift74HC595 * const self, void (*latch)(void), uint8_t number_of_chips) {
  self->number_of_chips = number_of_chips;
  self->latch = latch;
}

void shift74HC595_out(shift74HC595 * const self, uint8_t *data) {
  for (int i=self->number_of_chips; i>=0; i--) {
    SPI_masterTransmit(data[i]);
  }
  self->latch();
}

void shift74HC595_reset(shift74HC595 * const self) {
  for (int i=0; i<self->number_of_chips; i++) {
    SPI_masterTransmit(0x00);
  }
  self->latch();
}


/* ------------------------------- */
/*             74HC165             */
/* ------------------------------- */

void shift74HC165_init(shift74HC165 * const self, void (*set_latch)(uint8_t state), uint8_t number_of_chips) {
  self->number_of_chips = number_of_chips;
  self->set_latch = set_latch;
}


void shift74HC165_read(shift74HC165 * const self, uint8_t *receive_data) {
  self->set_latch(HIGH);
  _delay_us(10);
  for (int i=0; i<self->number_of_chips; i++) {
    receive_data[i] = SPI_masterReceive();
  }
  _delay_us(10);
  self->set_latch(LOW);
  
}