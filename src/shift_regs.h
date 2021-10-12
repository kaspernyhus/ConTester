#ifndef SHIFTREGS_H_
#define SHIFTREGS_H_

#include <stdio.h>
#include "SPI.h"

#define LOW 0
#define HIGH 1

/* ------------------------------- */
/*             74HC595             */
/* ------------------------------- */
typedef struct {
  uint8_t number_of_chips;
  void (*latch)(void);
} shift74HC595_s;

void shift74HC595_init(shift74HC595_s * const self, void (*latch)(void), uint8_t number_of_chips);
void shift74HC595_out(shift74HC595_s * const self, uint8_t *data);
void shift74HC595_reset(shift74HC595_s * const self);



/* ------------------------------- */
/*             74HC165             */
/* ------------------------------- */
typedef struct {
  uint8_t number_of_chips;
  void (*set_latch)(uint8_t state);
} shift74HC165_s;

void shift74HC165_init(shift74HC165_s * const self, void (*set_latch)(uint8_t state), uint8_t number_of_chips);
void shift74HC165_read(shift74HC165_s * const self, uint8_t *receive_data);

#endif /* SHIFTREGS_H_ */