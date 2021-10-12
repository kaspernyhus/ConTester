#include "terminal_matrix.h"


const char matrix_title[] PROGMEM = "\n \
                      _____         _______        _             \n \
                     / ____|       |__   __|      | |            \n \
                    | |     ___  _ __ | | ___  ___| |_ ___ _ __  \n \
                    | |    / _ \\| '_ \\| |/ _ \\/ __| __/ _ \\ '__| \n \
                    | |___| (_) | | | | |  __/\\__ \\ ||  __/ |    \n \
                     \\_____\\___/|_| |_|_|\\___||___/\\__\\___|_|    v1.0 \n \
                                       \n \
";
const char matrix_header[] PROGMEM = "\
  +------+------------------------------------------------------------------------+ \n \
  |      |                               INPUTS                                   | \n \
  |      | 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 | \n \
  +------+------------------------------------------------------------------------+ ";
const char matrix_bottom[] PROGMEM = "\n \
  +------+------------------------------------------------------------------------+ \n\n\n";




/* Function to generate each connection line in the matrix with variable connections */
/* line: return string */
/* line number: number in the output column */
/* data: connection data in bytes */
/* number_of_chips: how many bytes in data */
void generate_line(char *line, uint8_t line_number, uint8_t *data, uint8_t number_of_chips) {
  char buffer[20];
  strcpy(line,"\n   | ");

  switch (line_number) {
  case 9:
    strcat(line,"O");
    break;
  case 10:
    strcat(line,"U");
    break;
  case 11:
    strcat(line,"T");
    break;
  case 12:
    strcat(line,"P");
    break;
  case 13:
    strcat(line,"U");
    break;
  case 14:
    strcat(line,"T");
    break;
  case 15:
    strcat(line,"S");
    break;
  default:
    strcat(line," ");
    break;
  }
  sprintf(buffer," %2d |",line_number);
  strcat(line,buffer);
  /* generate the connection plots in chunks of 8bits = number of chips */
  for(int i=0; i<number_of_chips; i++) {
    for(int j=0; j<8;j++) {
      if(((data[i]>>j) & 1) == 0) strcat(line, " - "); // shift data -> mask with 1 -> bit is either 0 or 1
      else strcat(line," X ");
    }
  }
  strcat(line,"| ");
}