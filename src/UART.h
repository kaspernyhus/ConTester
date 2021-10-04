#ifndef UART_H_
#define UART_H_

/*
    
    Kasper Nyhus Kaae
        s195087

---------------------------------
UART module for
ARDUINO MEGA2560
---------------------------------
*/

#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>


void UART0_Init(unsigned int baud);
void UART0_sendChar(char data);
char UART0_receiveChar();
void UART0_puts(char *string);
void UART0_gets(char *c_buffer);
void UART0_enableReceive_Itr();
void UART0_disableReceive_Itr();

void UART1_Init(unsigned int baud);
void UART1_sendChar(char data);
char UART1_receiveChar();
void UART1_puts(char *string);
void UART1_gets(char *c_buffer);
void UART1_enableReceive_Itr();
void UART1_disableReceive_Itr();

#endif /* UART_H_ */