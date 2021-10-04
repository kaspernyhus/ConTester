#include "RotaryEncoder.h"

static int8_t rot_enc_table[] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};


void RotEnc_init(RotEnc * const self, uint8_t (*readA)(), uint8_t (*readB)(), uint8_t (*readBtnPin)(void), uint8_t BTN_debounce_ticks, uint16_t BTN_long_press_ticks, void (*onBtnPress)(void)) {
  self->readA = readA;
  self->readB = readB;
  self->_PSNS = 0;
  self->_store = 0;
  
  /* BTN super class */
  BTN_init(&self->RotEncBTN, readBtnPin, BTN_debounce_ticks, BTN_long_press_ticks);
  BTN_attachPress(&self->RotEncBTN, onBtnPress);
}

void RotEncBTN_attatchPress(RotEnc * const self, void (*onPress)(void)) {
  BTN_attachPress(&self->RotEncBTN, onPress);
}

void RotEnc_BTN_attatchLongPress(RotEnc * const self, void (*onLongPress)(void)) {
  BTN_attachLongPress(&self->RotEncBTN, onLongPress);
}

void RotEnc_AttachOnCW(RotEnc * const self, void (*functionPointer)()) {
  self->onCWmove = functionPointer;
}
void RotEnc_AttachOnCCW(RotEnc * const self, void (*functionPointer)()) {
  self->onCCWmove = functionPointer;
}


// return direction, 0 when no movement
int8_t RotEnc_read(RotEnc * const self) {
  BTN_tick(&self->RotEncBTN);

  self->_PSNS <<= 2;                              // shift previous state bits = ABXX
  if ( !!(self->readA()) ) self->_PSNS |= 0x02;   // read pinA; if A=1, set bit #1
  if ( !!(self->readB()) ) self->_PSNS |= 0x01;   // read pinB; if B=1, set bit #0
  self->_PSNS &= 0x0F;                            // mask out old bits = 0b0000PSNS
  
  if (rot_enc_table[self->_PSNS] ) {       // if 1 or -1
    self->_store <<= 4;
    self->_store |= self->_PSNS;
    if ((self->_store&0xFF) == 0x2B) {     // 0b00101011
      if ( self->onCCWmove != 0 ) {        // make sure there is an attatched callback function
        self->onCCWmove();
      }
      return -1;
    }
    if ((self->_store&0xFF) == 0x17) {     // 0b00010111
      if ( self->onCWmove != 0 ) {
        self->onCWmove();
      }
      return 1;
    }
  }
  return 0;
}
