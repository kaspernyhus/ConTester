#include "conTester.h"


const uint8_t correct_cons_1[][3] PROGMEM = {
      {0x01,0x00,0x00},
      {0x02,0x00,0x00},
      {0x04,0x00,0x00},
      {0x08,0x00,0x00},
      {0x10,0x00,0x00},
      {0x20,0x00,0x00},
      {0x40,0x00,0x00},
      {0x80,0x00,0x00},
      {0x00,0x01,0x00},
      {0x00,0x02,0x00},
      {0x00,0x04,0x00},
      {0x00,0x08,0x00},
      {0x00,0x10,0x00},
      {0x00,0x20,0x00},
      {0x00,0x40,0x00},
      {0x00,0x80,0x00},
      {0x00,0x00,0x01},
      {0x00,0x00,0x02},
      {0x00,0x00,0x04},
      {0x00,0x00,0x08},
      {0x00,0x00,0x10},
      {0x00,0x00,0x20},
      {0x00,0x00,0x40},
      {0x00,0x00,0x80},
      };


/* Private functions */
void _set_595_Output(ConTester_s * const self, uint8_t pinToSet);
void _read_165_Input(ConTester_s * const self, uint8_t *read_data);
void _scan(ConTester_s * const self, uint8_t scan_data[][NUMBER_OF_CHIPS]);



/* Public */
void ConTester_init(ConTester_s * const self, void (*latch595)(void), void (*latch165)(uint8_t state)) {
  shift74HC595_init(&self->shiftOUT,latch595,NUMBER_OF_CHIPS);
  shift74HC595_reset(&self->shiftOUT);
  shift74HC165_init(&self->shiftIN,latch165,NUMBER_OF_CHIPS);
  LabView_Init();
}

void ConTester_scan(ConTester_s * const self) {
  shift74HC595_reset(&self->shiftOUT); 
  _scan(self, self->scan_data);
}


void ConTester_setPinOut(ConTester_s * const self, uint8_t pinToSet) {
  shift74HC595_reset(&self->shiftOUT);
  _set_595_Output(self, pinToSet);
}


uint8_t ConTester_test(ConTester_s * const self) {
  uint8_t _test_byte;
  for (int i = 0; i < 24; i++) {
    for(int j=0; j<3; j++) {
      _test_byte = pgm_read_byte(&(correct_cons_1[i][j]));
      if(_test_byte != self->scan_data[i][j]) return 0;
    }  
  }
  return 1;
}


uint8_t ConTester_displayConErrors(ConTester_s * const self) {
  char buffer[100];
  uint8_t _test_byte;
  
  UART0_puts("   ---------------------------------------------\
              \n      Faulty connections\n   ---------------------------------------------\n");
  for (int i = 0; i < 24; i++) {
    for(int j=0; j<3; j++) {
      _test_byte = pgm_read_byte(&(correct_cons_1[i][j]));
      if(_test_byte != self->scan_data[i][j]) {               // Error found
        uint8_t error_located = 0;
        /* search for inputs in all 3 chips */
        for(int k=0; k<3; k++) {
          /* search each bit in the byte */
          for(int l=0; l<8; l++) {                              // checking each bit in the byte
            if((self->scan_data[i][k]>>l)&1) {                  // if bit set = there is a connecton
              // sprintf(buffer,"k: %d, l: %d \n",k,l);
              // UART0_puts(buffer);                                  
              sprintf(buffer,"      Output %2d connected to input %2d\n",i+1,(k*8)+l+1);
              UART0_puts(buffer);
              error_located = 1;
            }
          }
        }
        if(!error_located) {
          sprintf(buffer,"      Output %2d not connected\n",i+1);
          UART0_puts(buffer);
        }
      }
    }  
  }
  return 1;
}



// --------------------- Display scan results ------------------------- //

void ConTester_sendToLabView(ConTester_s * const self) {
  sendToLabview(self->scan_data);
}


void read_string_from_flash(const char *flash, uint16_t len, char *string) {
  char _char;
  _char = pgm_read_byte_near(flash);
  string[0] = _char;
  for (int k = 1; k < len; k++) {
    _char = pgm_read_byte_near(flash + k);
    string[k] = _char;
  }     
}


void ConTester_printToTerminal(ConTester_s * const self) {
  char line[100];
  char buffer[500];
  uint8_t temp[NUMBER_OF_CHIPS];
  
  UART0_sendChar(0x0C);                           // Clears CoolTerm !

  read_string_from_flash(matrix_title,500,buffer);   
  UART0_puts(buffer);                             // Prints title
  
  read_string_from_flash(matrix_header,400,buffer);
  UART0_puts(buffer);                             // prints header

  /* prints matrix body */
  for(int i=0; i<NUMBER_OF_CONNECTIONS; i++) {    // the number of rows in the matrix                 
    // the number of data bytes in each row
    // copy row to temporary array to pass on
    for(int j=0; j<NUMBER_OF_CHIPS; j++) temp[j] = self->scan_data[i][j];                          
    generate_line(line,i+1,temp,NUMBER_OF_CHIPS);  // generates a full line from data
    UART0_puts(line);
  }
  read_string_from_flash(matrix_bottom,100,buffer);
  UART0_puts(buffer);                             // ends matrix
}




// --------------------- Private ------------------------- //

void _set_595_Output(ConTester_s * const self, uint8_t pinToSet) {
  uint8_t shiftmask[NUMBER_OF_CHIPS];
  for(int i=0;i<NUMBER_OF_CHIPS;i++) {
    shiftmask[i] = (1<<(pinToSet-(8*i)))>>1; // convert uint value to shiftmask[] array of 8bits.
  }
  shift74HC595_out(&self->shiftOUT,shiftmask);
}


void _read_165_Input(ConTester_s * const self, uint8_t *read_data) {
  shift74HC165_read(&self->shiftIN,read_data);
}


void _scan(ConTester_s * const self, uint8_t scan_data[][NUMBER_OF_CHIPS]) { // scan_data[iteration][number of chips]
  uint8_t temp_data[NUMBER_OF_CHIPS];

  // There are 24 connections to test pr board.
  // Algorithm: Set one bit on OUTPUT -> save data from all INPUT chips
  for(int i=0; i<NUMBER_OF_CONNECTIONS; i++) {        
    _set_595_Output(self, i+1);
    _read_165_Input(self, temp_data);
    for(int j=0; j<NUMBER_OF_CHIPS; j++) {
      scan_data[i][j] = temp_data[j];
    }
  }
}