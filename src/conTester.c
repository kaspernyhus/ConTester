#include "conTester.h"


uint32_t test_data[] = {
      0x00000001,
      0x00000002,
      0x00000004,
      0x00000008,
      0x00000010,
      0x00000020,
      0x00000040,
      0x00000080,
      0x00000100,
      0x00000200,
      0x00000400,
      0x00000800,
      0x00001000,
      0x00002000,
      0x00004000,
      0x00008000,
      0x00010000,
      0x00020000,
      0x00040000,
      0x00070000,
      0x00100000,
      0x00200000,
      0x00400000,
      0x00800000
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



// --------------------- Display scan results ------------------------- //

void ConTester_sendToLabView(ConTester_s * const self) {
  sendToLabview(self->scan_data);
}

void reset_buffer(char *data, uint8_t len) {
  for(int i=0; i<len; i++) {
    data[i] = 0;
  }
}


void ConTester_printToTerminal(ConTester_s * const self) {
  char line[100];
  UART0_sendChar(0x0C);             // Clears CoolTerm !
  UART0_puts(matrix_title);         // Prints title
  UART0_puts(matrix_header);        // prints header
  for(int i=0; i<24; i++) {         // prints matrix body
    reset_buffer(line,100);
    generate_line(line,i+1,test_data[i]);
    UART0_puts(line);
  }
  UART0_puts(matrix_bottom);        // ends matrix
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