#ifndef BUTTON_H_
#define BUTTON_H_

/*

Pushbutton module
v1.0
*/



#include <stdio.h>
#include <stdint.h>

#define FALSE 0
#define TRUE 1

/* BTN State Machine states */
enum states {
  LISTEN,
  WAIT,
  CHECK,
  LONG,
  DOUBLE  
};


/* BTN class */
typedef struct {
  uint8_t _push_detected;
  uint8_t _state;
  uint16_t _tick_timer;

  uint8_t debounce_ticks;
  uint16_t long_press_ticks;
  uint8_t (*readPin)(void);
  void (*onPress)(void);
  void (*onLongPress)(void);
  void (*onDoublePress)(void);
} BTN;


void BTN_init(BTN * const self, uint8_t (*readPin)(void), uint8_t debounce_ticks, uint16_t long_press_ticks);
void BTN_attachPress(BTN * const self, void (*onPress)(void));
void BTN_attachLongPress(BTN * const self, void (*onLongPress)(void));
void BTN_tick(BTN * const self);



#endif /* BUTTON_H_ */