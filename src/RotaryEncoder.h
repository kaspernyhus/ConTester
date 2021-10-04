#ifndef ROTARY_H_
#define ROTARY_H_
/*
---------------------------------
Rotary Encoder module for
ARDUINO NANO
v1.0
---------------------------------
*/

/*

    Previous State - Next State
      (previous AB - next AB)
 ----------------------------------
|   PSNS   |  Valid  |  Direction  |
|–---------------------------------|
|   0000   |    X    |      X      |
|   0001   |  Valid  |     CW      |
|   0010   |  Valid  |     CCW     |
|   0011   |    X    |      X      |
|   0100   |  Valid  |     CCW     |
|   0101   |    X    |      X      |
|   0110   |    X    |      X      |
|   0111   |  Valid  |     CW      |
|   1000   |  Valid  |     CW      |
|   1001   |    X    |      X      |
|   1010   |    X    |      X      |
|   1011   |  Valid  |     CCW     |
|   1100   |    X    |      X      |
|   1101   |  Valid  |     CCW     |
|   1110   |  Valid  |     CW      |
|   1111   |    X    |      X      |
 –---------------------------------

0=X, 1=CW, -1=CCW

rot_enc_table[]= {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};

*/



#include <stdio.h>
#include "bsp.h"
#include "button.h"


typedef struct {
  uint8_t _PSNS;             // Previous State - Next State
  uint8_t _store;
  uint8_t (*readA)(void);
  uint8_t (*readB)(void);
  void (*onCWmove)(void);
  void (*onCCWmove)(void);
  BTN RotEncBTN;
} RotEnc;


void RotEnc_init( RotEnc * const self, 
                  uint8_t (*readA)(), 
                  uint8_t (*readB)(), 
                  uint8_t (*readBtnPin)(void), 
                  uint8_t BTN_debounce_ticks, 
                  uint16_t BTN_long_press_ticks, 
                  void (*onBtnPress)(void)
                );
void RotEnc_BTN_attatchPress(RotEnc * const self, void (*onPress)(void));
void RotEnc_BTN_attatchLongPress(RotEnc * const self, void (*onLongPress)(void));
void RotEnc_AttachOnCW(RotEnc * const self, void (*functionPointer)());
void RotEnc_AttachOnCCW(RotEnc * const self, void (*functionPointer)());

int8_t RotEnc_read(RotEnc * const self);


#endif /* ROTARY_H_ */