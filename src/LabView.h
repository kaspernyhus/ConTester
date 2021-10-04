
#ifndef LABVIEW_H_
#define LABVIEW_H_


/*
    
    Kasper Nyhus Kaae
        s195087

---------------------------------
LabView COM module for
ARDUINO MEGA2560
---------------------------------

*/

#include <stdio.h>
#include "UART.h"


void LabView_Init();
void sendToLabview(uint8_t data[][3]);


#endif /* LABVIEW_H_ */