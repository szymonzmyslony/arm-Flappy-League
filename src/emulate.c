#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>

#define NUMBER_OF_REGISTERS   17
#define BYTES_IN_MEMORY       65536
// index of Program Counter in registers array
#define PC                    15
// index of CPSR in registers array
#define CPSR                  16

struct processor {
  uint32_t registers[NUMBER_OF_REGISTERS];	 
  uint8_t memory[BYTES_IN_MEMORY];
  bool end;
  int counter; // counter to detewrmine whether to decode and execute
};

struct arguments {
  uint8_t dRegIndex;
  uint8_t sRegIndex;
  uint8_t nRegIndex;
  uint8_t mRegIndex;
  uint16_t operand2;
  uint8_t cond;
  uint32_t offset;
  void (*executePointer)(struct arguments args); 
};

void decode(uint32_t dInstruction, struct arguments decodedArgs);

int main(int argc, char **argv) {
  
  struct processor arm;
  struct arguments decodedArgs;

  // load program from args and initialise processor and read from file


  // points to appropriate execute function after decoding
  uint32_t dInstruction; // instruction to be decoded next

  while (!arm.end) {
    
    // execute instruction
    if (arm.counter >= 2) {
      execute(decodedArgs, arm);
    }

    // decode instruction
    if (arm.counter >= 1) {
      decode(dInstruction, decodedArgs);
    }

    // fetch instruction
    dInstruction = fetch(dInstruction, arm);

    // Increment counter but avoid overflow of counter
    if (counter < 3){
      arm.counter++;
    }
  }
  
  // print register states

  return EXIT_SUCCESS;
}

void decode(uint32_t dInstruction, struct arguments decodedArgs){
  // set mask to bit 27
  uint32_t mask = 0x08000000;
  if ((dInstruction & mask) != 0){
    // Decode Branch instruction
    
    return;
  }

  // set mask to bit 26
  mask = 0x04000000;
  if ((dInstruction & mask) != 0){
    // Decode Single Data Transfer

    return;
  }

  // set mask to bit 25
  mask = 0x02000000;
  if ((dInstruction & mask) != 0){
    // Decode Data Processing (I = 1)

    return;
  }

  // set mask to bit 4
  mask = 0x00000010;
  if ((dInstruction & mask) == 0){
    // Decode Data Processing (I = 0 Constant Shift)

    return;
  }

  // set mask to bit 7
  mask = 0x00000080;
  if ((dInstruction & mask) != 0){
    // Decode Multiply

    return;
  } else {
    // Decode Data Processing (I = 0 Shift by Register)

    return;
  }
}


