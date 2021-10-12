#ifndef TERMINAL_MATRIX_H_
#define TERMINAL_MATRIX_H_


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <avr/pgmspace.h>


const char matrix_title[500];
const char matrix_header[400];
const char matrix_bottom[100];

void generate_line(char *line, uint8_t line_number, uint8_t *data, uint8_t number_of_chips);
void generate_con_line(uint8_t *data, uint8_t length, char line[80]);


#endif /* TERMINAL_MATRIX_H_ */