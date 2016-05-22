#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include "emulate.h"

#define NUMBER_OF_REGISTERS   17
#define BYTES_IN_MEMORY       65536
#define INSTRUCTION_BYTES      4
// index of Program Counter in registers array
#define PC                    15
// index of CPSR in registers array
#define CPSR                  16
// bit flag indexes
#define Vbit                  28
#define Cbit                  29
#define Zbit                  30
#define Nbit                  31

// masks for multiply
#define Abit                  21
#define Sbit                  20
                  
// masks
#define MASK3_0               0x0000000F
#define MASK11_8              0x00000F00
#define MASK15_12             0x0000F000
#define MASK19_16             0x000F0000
#define MASK24_21             0x01E00000

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
  uint16_t operand2;
  uint8_t cond;
  uint32_t offset;
  bool aFlag;
  bool sFlag;
  bool iFlag;
  void (*executePointer)(struct arguments args, struct processor arm); 
};


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
    dInstruction = fetch(arm);
    
    // increment program counter 
    arm.registers[PC] += INSTRUCTION_BYTES;

    // Increment counter but avoid overflow of counter
    if (arm.counter < 3){
      arm.counter++;
    }
  }
  
  // print register states

  return EXIT_SUCCESS;
}

// ======================= Decode Multiply ====================================
void decodeMul(int dInstruction, struct arguments *decodedArgs) {
  //Decode Rd
  uint32_t dMask = MASK19_16;
  decodedArgs->dRegIndex = (dInstruction & dMask) >> 16;

  //Decode Rn
  uint32_t nMask = MASK15_12;
  decodedArgs->nRegIndex = (dInstruction & nMask) >> 12;

  //Decode Rs
  uint32_t sMask = MASK11_8;
  decodedArgs->sRegIndex = (dInstruction & sMask) >> 8;

  //Decode Rm
  uint32_t mMask = MASK3_0;
  decodedArgs->mRegIndex = (dInstruction & mMask);

  //Set sFlag
  uint32_t sBitMask = 1 << Sbit;
  bool sFlag = (dInstruction & sBitMask) == sBitMask;
  decodedArgs->sFlag = sFlag;

  //Set aFlag
  uint32_t aMask = 1 << Abit;
  decodedArgs->aFlag = (dInstruction & aMask) == aMask;
  
  decodedArgs->executePointer = &mul; 
}



// ======================= Execute Multiply ===================================
void mul(struct arguments *decodedArgs, struct processor *arm) {
  uint32_t res = arm->registers[decodedArgs->mRegIndex] * arm->registers[decodedArgs->sRegIndex];
  if (decodedArgs->aFlag == true) {
    res = res + arm->registers[decodedArgs->nRegIndex];
  }
  if (decodedArgs->sFlag == true) {
    setFlagsMul(res, arm);
  }
  *(arm->registers[decodedArgs->dRegIndex]) = res;
}


// ======================= Set Flags ==========================================
void setFlagsMul(uint32_t value, struct processor arm) {
  //Set N flag
  uint32_t bit31 = 1 << 31;
  bool bit31set = (value & bit31) == bit31;
  *(arm->registers[CPSR]) = setBit(arm->registers[CPSR], bit31set, Nbit);

  //Set Z flag
  bool allZero = value == 0;
  *(arm->registers[CPSR]) = setBit(arm->registers[CPSR], allZero, Zbit);
}

// Returns the instruction in the byte order as shown in the specification
// and increments the program counter
uint32_t fetch(struct processor *arm) {
  return   (arm->memory[arm->registers[PC] + 3] << 24) 
          + (arm->memory[arm->registers[PC] + 2] << 16)
          + (arm->memory[arm->registers[PC] + 1] <<  8)
          +  arm->memory[arm->registers[PC]];
}

// ====================== Helper Functions ====================================

// Returns a given word except with a single bit set in the given position
uint32_t setBit(uint32_t word, bool set, uint8_t position) {
  //Clears the bit at the given position and then sets it
  return (word & ~(1 << position)) | (set << position);
}
