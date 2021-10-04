#ifndef BSP_H_
#define BSP_H_

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#define LOW 0
#define HIGH 1

/* Shift registers */
#define LATCH_595 PB2
#define LATCH_165 PB1
void configure_latch_pins();
void latch595();
void latch165(uint8_t state);


/* Rotary encoder */
#define RotPinA PD3
#define RotPinB PD2
#define RotBtnPin PD4
void configure_RotEnc_pins();
void configure_RotEnc_button_pin();
uint8_t EncRot_read_pin_A();
uint8_t EncRot_read_pin_B();
uint8_t EncRot_read_BTN_pin();






#endif /* BSP_H_ */