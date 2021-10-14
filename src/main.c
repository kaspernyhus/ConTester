/*
 -----------------
   ARDUINO NANO
 -----------------
 
  NH ConTester v1.0

*/ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "bsp.h"
#include "external_interrupts.h"
#include "I2C.h"
#include "RotaryEncoder.h"
#include "SPI.h"
#include "ssd1306.h"
#include "timer.h"
#include "UART.h"
#include "button.h"
#include "conTester.h"


/* UART */
#define BAUD 115200
#define MYUBRR F_CPU/8/BAUD-1 //full dublex

#define TERMINAL 0
#define LABVIEW 1
#define OUTPUT_RESULT_TO TERMINAL


/* sys tick timer */
volatile uint8_t ms_tick = 0;

/* Encoder */
RotEnc Encoder;

/* Board class */
ConTester_s ConTester;

/* Function declarations */
void init();
void run_scan();
char buffer[20];


int main() {
  init();
  
  while(1) {    
    if(ms_tick) {
      ms_tick = 0;
      RotEnc_read(&Encoder);
    }
  }
}


void init() {
  /* Sys tick timer */
  timer0_8bit_Init(PRESCALE_64,CTC_MODE,249);
  timer0_itr_config(ENABLE,COMPARE_MATCH_A);
  
  /* UART */
  UART0_Init(MYUBRR);
  UART0_puts("UART initialized\r\n");
  
  /* SPI init */  
  SPI_master_init();
  SPI_set_data_order(0);

  /* ConTester board */
  configure_latch_pins();
  ConTester_init(&ConTester,&latch595,&latch165);
  
  /* Display */
  I2C_Init();
  InitializeDisplay();
  clear_display();
  sendStrXY("ConTester v1.0",0,1);

  /* Rotary encoder */
  configure_RotEnc_pins();
  configure_RotEnc_button_pin();
  RotEnc_init(&Encoder, &EncRot_read_pin_A, &EncRot_read_pin_B, &EncRot_read_BTN_pin, 20, 1000, &run_scan);

  sei();
}


void run_scan() {
  ConTester_scan(&ConTester);
  #if OUTPUT_RESULT_TO == LABVIEW
    ConTester_sendToLabView(&ConTester);
  #else
    ConTester_printToTerminal(&ConTester);
  #endif
  
  uint8_t test_result = ConTester_test(&ConTester);
  if(test_result) {
    clear_display();
    sendStrXY("ConTester v1.0",0,1);
    sendStrXY("All OK!",3,5);
  }
  else {
    clear_display();
    sendStrXY("ConTester v1.0",0,1);
    sendStrXY("Error detected!",3,1);
    ConTester_displayConErrors(&ConTester);
  }
}


ISR(TIMER0_COMPA_vect) {
  ms_tick = 1;
}