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
#include "LabView.h"
#include "button.h"
#include "conTester.h"


/* UART */
#define BAUD 9600
#define MYUBRR F_CPU/8/BAUD-1 //full dublex



/* Encoder */
RotEnc Encoder;

uint8_t shiftmask[3];
uint16_t pinOn = 0;
char buffer[20];


/* Function declarations */
void init();


void inc() {
  pinOn++;
  sprintf(buffer,"%3d",pinOn);
  sendStrXY(buffer,1,12);
  set_595_Output(pinOn);
}

void dec() {
  pinOn--;
  sprintf(buffer,"%3d",pinOn);
  sendStrXY(buffer,1,12);
  set_595_Output(pinOn);
}

void reset() {
  pinOn = 0;
  sprintf(buffer,"%3d",pinOn);
  sendStrXY(buffer,1,12);
  for(int i=0; i<3;i++) {
        shiftmask[i] = 0;
      }
  reset_595s();
}






int main() {
  init();

  // uint8_t scan_data[8][3] =  {
  //                             {0x00,0x00,0x00},
  //                             {0x00,0x00,0x00},
  //                             {0x00,0x00,0x00},
  //                             {0x00,0x00,0x00},
  //                             {0x00,0x00,0x00},
  //                             {0x00,0x00,0x00},
  //                             {0x00,0x00,0x00},
  //                             {0x00,0x00,0x00},
  //                           };
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
  //UART0_puts("UART initialized\r\n");
  
  /* SPI init */  
  SPI_master_init();
  SPI_set_data_order(0);

  /* shift registers init */
  configure_latch_pins();
  ConTester_init(&latch595,&latch165);
  reset_595s();

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
  RotEnc_AttachOnCCW(&Encoder, &dec);
  RotEnc_BTN_attatchLongPress(&Encoder, &reset);
  
  //sei();
}