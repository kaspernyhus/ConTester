#ifndef CONTESTER_H_
#define CONTESTER_H_

#include "shift_regs.h"
#include "LabView.h"
#include "terminal_matrix.h"


#define NUMBER_OF_BOARDS 1
#define NUMBER_OF_CHIPS NUMBER_OF_BOARDS*3
#define NUMBER_OF_CONNECTIONS NUMBER_OF_BOARDS*24


typedef struct {
  uint8_t scan_data[NUMBER_OF_CONNECTIONS][NUMBER_OF_CHIPS];
  shift74HC595_s shiftOUT;
  shift74HC165_s shiftIN;
} ConTester_s;

void ConTester_init(ConTester_s * const self, void (*latch595)(void), void (*latch165)(uint8_t state));
void ConTester_scan(ConTester_s * const self);
void ConTester_sendToLabView(ConTester_s * const self);
void ConTester_printToTerminal(ConTester_s * const self);
void ConTester_setPinOut(ConTester_s * const self, uint8_t pinToSet);
uint8_t ConTester_test(ConTester_s * const self);
uint8_t ConTester_displayConErrors(ConTester_s * const self);

#endif /* CONTESTER_H_ */