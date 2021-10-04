#include "bsp.h"



void configure_latch_pins() {
  DDRB |=  (1<<LATCH_595) | (1<<LATCH_165);   // OUTPUT
  PORTB &=~(1<<LATCH_595) | (1<<LATCH_165);
}

void latch595() {
  PORTB |= (1<<LATCH_595);
  _delay_us(10);
  PORTB &=~ (1<<LATCH_595);
}

void latch165(uint8_t state) {
  switch(state) {
  case LOW:
    PORTB &=~ (1<<LATCH_165);
    break;
  case HIGH:
    PORTB |= (1<<LATCH_165);
    break;
  default:
    PORTB &=~ (1<<LATCH_165);
    break;
  }
}


/* Rotary Encoder */
void configure_RotEnc_pins() {
  // Sets RotPins as input pull-up
  DDRD &=~ (1<<RotPinA)|(1<<RotPinB);
  PORTD |= (1<<RotPinA)|(1<<RotPinB);
}

void configure_RotEnc_button_pin() {
  DDRD &=~ (1<<RotBtnPin);
  PORTD |= (1<<RotBtnPin);
}

uint8_t EncRot_read_pin_A() {
  return PIND & (1<<RotPinA);
}
uint8_t EncRot_read_pin_B() {
  return PIND & (1<<RotPinB);
}
uint8_t EncRot_read_BTN_pin() {
  return PIND & (1<<RotBtnPin);
}