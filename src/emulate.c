#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "emulate.h"

#define NUMBER_OF_REGISTERS   17
#define BYTES_IN_MEMORY       65536
// index of Program Counter in registers array
#define PC                    15
// index of CPSR in registers array
#define CPSR                  16
// bit flag indexes
#define Vbit                  28
#define Cbit                  29
#define Zbit                  30
#define Nbit                  31
// masks
#define MASK15_12             0x0000F000
#define MASK19_16             0x000F0000
#define MASK24_21             0x01E00000
#define MASK11_8              0x00000f00
#define MASK11_7              0x00000f80
#define MASK7_0               0x000000ff
#define MASK6_5               0x00000060
#define MASK3_0               0x0000000f

struct processor {
  uint32_t registers[NUMBER_OF_REGISTERS];	 
  uint8_t memory[BYTES_IN_MEMORY];
  bool end;
  int counter; // counter to determine whether to decode and execute
};

struct arguments {
  uint8_t dRegIndex;
  uint8_t sRegIndex;
  uint8_t nRegIndex;
  uint8_t mRegIndex;
  uint32_t operand2;
  uint8_t cond;
  uint8_t i;
  uint32_t offset;
  void (*executePointer)(struct arguments *args, struct processor *arm); 
};

void resolveOperand2(uint16_t op, uint8_t i,
        struct arguments *decodedArgs, struct processor *arm);

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
    if (arm.counter < 3){
      arm.counter++;
    }
  }
  
  // print register states

  return EXIT_SUCCESS;
}

// ====================== Helper Functions ====================================

// Returns a given word except with a single bit set in the given position
uint32_t setBit(uint32_t word, bool set, uint8_t position) {
  //Clears the bit at the given position and then sets it
  return (word & ~(1 << position)) | (set << position);
}

// Rotates a 32 bit value, val right by n bits
uint32_t rotateRight32(uint32_t val, uint16_t n){
  return ((val >> n) | (val << (32 - n)));
}

// Arithmetically shift right a 32 bit value, val right by n bits
uint32_t arithShiftRight32(uint32_t val, uint16_t n){
  // if MSB is 0 then same as logical shift
  if ((val & 0x80000000) == 0){
    return (val >> n);
  } else {
  // if MSB is 1 then the n most sig bits must be set to 1
    uint32_t mask = ~((0x00000001 << (32 - (n + 1))) - 1);
    return (mask | (val >> n));
  }
}

// Resolves operand2 into an integer to be used in execution
// Should only be called during execution
void resolveOperand2(uint16_t op, uint8_t i,
        struct arguments *decodedArgs, struct processor *arm){
  // if i = 1 then immediate value
  if (i == 1){
    uint32_t rotation = (((MASK11_8 & op) >> 8) * 2);
    decodedArgs->operand2 = rotateRight32((op & MASK7_0), rotation);
  // if i = 0 then shifted register
  } else {
    // get number of reg to be shifted
    uint8_t reg = (MASK3_0 & op);
    // if bit 4 = 0 then integer shift
    if ((0x00000010 & op) == 0){
      decodedArgs->operand2 = shift((MASK6_5 & op) >> 5, arm->registers[reg], 
              ((MASK11_7 & op) >> 7));
    // if bit 4 = 1 then register shift
    } else {
      uint16_t rotateAmount = arm->registers[(MASK11_8 & op) >> 8];
      decodedArgs->operand2 = shift((MASK6_5 & op) >> 5, arm->registers[reg], 
              rotateAmount);
    }
  }
}

// shifts value according to shift code by n bits
uint32_t shift(uint8_t shiftCode, uint32_t value, uint16_t n){
  switch (shiftCode){
    case 0x00:
      return (value << n);
    case 0x01:
      return (value >> n);
    case 0x02:
      return arithShiftRight32(value, n);
    case 0x03:
      return rotateRight32(value, n);
    default:
      fprintf(stderr, "Invalid shift code %d to shift %d by %d", shiftCode,
              value, n);
      return 0;
  }
}
