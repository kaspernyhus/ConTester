#include "button.h"



void BTN_init(BTN * const self, uint8_t (*readPin)(void), uint8_t debounce_ticks, uint16_t long_press_ticks) {
  self->readPin = readPin;
  self->debounce_ticks = debounce_ticks;
  self->long_press_ticks = long_press_ticks;
  self->_state = 0;
  self->_tick_timer = 0;
  self->_push_detected = FALSE;
}

void BTN_attachPress(BTN * const self, void (*onPress)(void)) {
  self->onPress = onPress;
}

void BTN_attachLongPress(BTN * const self, void (*onLongPress)(void)) {
  self->onLongPress = onLongPress;
}


void BTN_tick(BTN * const self) {     // must be called at interval ≈ 1ms
  self->_tick_timer++;

  if(self->_push_detected == 0) {     // if no recent BTN push detected, read pin
    self->_push_detected = self->readPin();
  }
  
  if(self->_push_detected) {           // push recently detected
    
    if(self->_state == LISTEN) {       // restart _timer, goto _state 1
      self->_tick_timer = 0;
      self->_state = WAIT;
    }

    if(self->_state == WAIT) {         // stay in _state 1 till debounced (eg. 20 * 1ms _tick), goto _state 2
      if (self->_tick_timer > self->debounce_ticks) {
        self->_state = CHECK;
      }
    }

    if(self->_state == CHECK) {        // check if button is still pressed
      if ( !(self->readPin()) ) {
        self->onPress();
        self->_state = LONG;           // begin check if long press     
      }
      else {
        self->_push_detected = FALSE;  // no valid button press detected
        self->_state = LISTEN;
      }
    }

    if (self->_state == LONG) {                    
      if ( !!(self->readPin()) ) {     // if during _state 3 button is released -> reset
        self->_push_detected = FALSE;
        self->_state = LISTEN;
      }
      if (self->_tick_timer >= self->long_press_ticks) {      // check if long press time reached (eg. 1000 * 1ms _tick)
        self->onLongPress();
        self->_push_detected = FALSE;   // reset
        self->_state = LISTEN;
      }
    }

    if (self->_state == DOUBLE) {
      self->_state = LISTEN;
    }
  }
}