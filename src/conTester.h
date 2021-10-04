#ifndef CONTESTER_H_
#define CONTESTER_H_

#include "shift_regs.h"


#define NUMBER_OF_BOARDS 1


void ConTester_init(void (*latch595)(), void (*latch165)(uint8_t state));
void set_595_Output(uint8_t pinToSet);
void reset_595s();


#endif /* CONTESTER_H_ */