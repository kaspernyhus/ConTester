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


/* Encoder */
RotEnc Encoder;

// uint8_t shiftmask[3];
// uint8_t read_data[3];
uint16_t pinOn = 0;
char buffer[20];


/* Board class */
ConTester_s ConTester;


/* Function declarations */
void init();


void inc() {
  pinOn++;
  sprintf(buffer,"%3d",pinOn);
  sendStrXY(buffer,1,12);
  ConTester_scan(&ConTester);
  #if OUTPUT_RESULT_TO == LABVIEW
    ConTester_sendToLabView(&ConTester);
  #else
    ConTester_printToTerminal(&ConTester);
  #endif
}

// void dec() {
//   pinOn--;
//   sprintf(buffer,"%3d",pinOn);
//   sendStrXY(buffer,1,12);
//   set_595_Output(pinOn);
// }

// void reset() {
//   pinOn = 0;
//   sprintf(buffer,"%3d",pinOn);
//   sendStrXY(buffer,1,12);
//   for(int i=0; i<3;i++) {
//         shiftmask[i] = 0;
//       }
//   reset_595s();
// }




int main() {
  init();

  uint16_t timer=0;
  
  while(1) {
    // scan(scan_data);
    // sendToLabview(scan_data);
    // _delay_ms(1000);
    
    timer++;

    RotEnc_read(&Encoder);
    
    if(timer>250) {
      timer = 0;
      // for(int i=0; i<3;i++) {
      //   sprintf(buffer,"%3d",shiftmask[i]);
      //   sendStrXY(buffer,i+2,2);
      // }
    }

    _delay_ms(1);

  }
}



void init() {
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
  RotEnc_init(&Encoder, &EncRot_read_pin_A, &EncRot_read_pin_B, &EncRot_read_BTN_pin, 20, 1000, &inc);
  RotEnc_AttachOnCW(&Encoder, &inc);
  // RotEnc_AttachOnCCW(&Encoder, &dec);
  // RotEnc_BTN_attatchLongPress(&Encoder, &reset);
  
  //sei();
}