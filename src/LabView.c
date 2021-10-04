/*
    
    Kasper Nyhus Kaae
        s195087

---------------------------------
LabView COM module for
ARDUINO MEGA2560
---------------------------------

*/

#include "LabView.h"


void LabView_Init() {
  
}


void sendToLabview(uint8_t data[][3]) {
  UART0_sendChar(0xA5);           // Sync byte
  

  for(int i=8;i>0;i--) {
    for(int j=3;j>0;j--) {
    UART0_sendChar(data[i-1][j-1]);
    }
  }
  
  
  UART0_sendChar(0x00);           // CRC
  UART0_sendChar(0x00);           // CRC
}

