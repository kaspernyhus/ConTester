#ifndef TERMINAL_MATRIX_H_
#define TERMINAL_MATRIX_H_


#include <stdio.h>
#include <stdint.h>
#include <string.h>

static char matrix_title[454];
static char matrix_header[339];
static char matrix_bottom[87];

void generate_line(char *line, uint8_t line_number, uint32_t data);
void generate_con_line(uint8_t *data, uint8_t length, char line[80]);


#endif /* TERMINAL_MATRIX_H_ */