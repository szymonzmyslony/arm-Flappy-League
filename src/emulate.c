#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
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

#define Sbit                  20
// masks
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
    dInstruction = fetch(dInstruction, arm);

    // Increment counter but avoid overflow of counter
    if (arm.counter < 3){
      arm.counter++;
    }
  }
  
  // print register states

  return EXIT_SUCCESS;
}

// ======================= Decode Data Processing =============================

void decodeDP(int dInstruction, struct arguments decodedArgs) {
  // - decode operation
  uint32_t mask = MASK24_21;
  //Remove the trailing 0s from the extracted opcode.
  uint8_t opCode = (dInstruction & mask) >> 21;
  
  uint32_t sMask = 1 << Sbit;
  bool sFlag = (dInstruction & sMask) == sMask;
  
  if(sFlag) {
    switch(opCode) {
      case  0: decodedArgs.executePointer = &opDPand; break;
      case  1: decodedArgs.executePointer = &opDPeor; break;
      case  2: decodedArgs.executePointer = &opDPsub; break;
      case  3: decodedArgs.executePointer = &opDPrsb; break;
      case  4: decodedArgs.executePointer = &opDPadd; break;
      case  8: decodedArgs.executePointer = &opDPtst; break;
      case  9: decodedArgs.executePointer = &opDPteq; break;
      case 10: decodedArgs.executePointer = &opDPcmp; break;
      case 12: decodedArgs.executePointer = &opDPorr; break;
      case 13: decodedArgs.executePointer = &opDPmov; break;
      //No such other opcode!
      default: assert(false); break;
    }
  } else {
    switch(opCode) {
      case  0: decodedArgs.executePointer = &opDPandWithFlags; break;
      case  1: decodedArgs.executePointer = &opDPeorWithFlags; break;
      case  2: decodedArgs.executePointer = &opDPsubWithFlags; break;
      case  3: decodedArgs.executePointer = &opDPrsbWithFlags; break;
      case  4: decodedArgs.executePointer = &opDPaddWithFlags; break;
      case  8:
      case  9:
      case 10: decodedArgs.executePointer = &opDPNothing; break;
      case 12: decodedArgs.executePointer = &opDPorrWithFlags; break;
      case 13: decodedArgs.executePointer = &opDPmov; break;
      //No such other opcode!
      default: assert(false); break;
    }
  }

  // - decode Rn
  mask = MASK19_16;
  decodedArgs.nRegIndex = (dInstruction & mask) >> 16;
  
  // - decode Rd
  mask = MASK15_12;
  decodedArgs.dRegIndex = (dInstruction & mask) >> 12;  
}

// ====================== Execute Data Processing =============================
// --- Logical Operations
// - And
void opDPand(struct arguments decodedArgs, struct processor arm) {
  uint32_t res = arm.registers[decodedArgs.nRegIndex] & decodedArgs.operand2;
  arm.registers[decodedArgs.dRegIndex] = res;
}

void opDPandWithFlags(struct arguments decodedArgs, struct processor arm) {
  opDPand(decodedArgs, arm);
  setFlagsDP(arm.registers[decodedArgs.dRegIndex], arm);
}

// - Eor
void opDPeor(struct arguments decodedArgs, struct processor arm) {
  uint32_t res = arm.registers[decodedArgs.nRegIndex] ^ decodedArgs.operand2;
  arm.registers[decodedArgs.dRegIndex] = res;
}

void opDPeorWithFlags(struct arguments decodedArgs, struct processor arm) {
  opDPeor(decodedArgs, arm);
  setFlagsDP(arm.registers[decodedArgs.dRegIndex], arm);
}

// - Or
void opDPorr(struct arguments decodedArgs, struct processor arm) {
  uint32_t res = arm.registers[decodedArgs.nRegIndex] | decodedArgs.operand2;
  arm.registers[decodedArgs.dRegIndex] = res;
}

void opDPorrWithFlags(struct arguments decodedArgs, struct processor arm) {
  opDPorr(decodedArgs, arm);
  setFlagsDP(arm.registers[decodedArgs.dRegIndex], arm);
}

// - Move
void opDPmov(struct arguments decodedArgs, struct processor arm) {
  arm.registers[decodedArgs.dRegIndex] = decodedArgs.operand2;
}

// --- Arithmetic Operations
// - Subtract (Rn - operand2)
void opDPsub(struct arguments decodedArgs, struct processor arm) {
  uint32_t res = arm.registers[decodedArgs.nRegIndex] - decodedArgs.operand2;
  arm.registers[decodedArgs.dRegIndex] = res;
}

void opDPsubWithFlags(struct arguments decodedArgs, struct processor arm) {
  opDPsub(decodedArgs, arm);
  
  setFlagsDP(arm.registers[decodedArgs.dRegIndex], arm);
  // Update flags for carry out
  bool borrow = !(decodedArgs.operand2 > arm.registers[decodedArgs.nRegIndex]);
  setBit(arm.registers[CPSR], borrow, Cbit);
}

// - Subtract (operand2 - Rn)
void opDPrsb(struct arguments decodedArgs, struct processor arm) {
  uint32_t res = decodedArgs.operand2 - arm.registers[decodedArgs.nRegIndex];
  arm.registers[decodedArgs.dRegIndex] = res;
}
  
void opDPrsbWithFlags(struct arguments decodedArgs, struct processor arm) {
  opDPrsb(decodedArgs, arm);
  
  setFlagsDP(arm.registers[decodedArgs.dRegIndex], arm);
  // Update flags for carry out
  bool borrow = !(arm.registers[decodedArgs.nRegIndex] > decodedArgs.operand2);
  setBit(arm.registers[CPSR], borrow, Cbit);
}

// - Add
void opDPadd(struct arguments decodedArgs, struct processor arm) {
  uint32_t res = decodedArgs.operand2 + arm.registers[decodedArgs.nRegIndex];
  arm.registers[decodedArgs.dRegIndex] = res;
}

void opDPaddWithFlags(struct arguments decodedArgs, struct processor arm) {
  opDPadd(decodedArgs, arm);
  
  setFlagsDP(arm.registers[decodedArgs.dRegIndex], arm);
  // Update flags for carry out
  bool overflow = arm.registers[decodedArgs.dRegIndex] 
                   < arm.registers[decodedArgs.nRegIndex]
                 || arm.registers[decodedArgs.dRegIndex] < decodedArgs.operand2;
  setBit(arm.registers[CPSR], overflow, Cbit);
}

// --- Testing operations
void opDPtst(struct arguments decodedArgs, struct processor arm) {
  uint32_t res = arm.registers[decodedArgs.nRegIndex] & decodedArgs.operand2;
  setFlagsDP(res, arm);
}

void opDPteq(struct arguments decodedArgs, struct processor arm) {
  uint32_t res = arm.registers[decodedArgs.nRegIndex] ^ decodedArgs.operand2;
  setFlagsDP(res, arm);
}

void opDPcmp(struct arguments decodedArgs, struct processor arm) {
  uint32_t res = arm.registers[decodedArgs.nRegIndex] - decodedArgs.operand2;
  setFlagsDP(res, arm);
}

//Replaces the test operations when the S flag = 0, as nothing changes
void opDPNothing(struct arguments decodedArgs, struct processor arm) {}

// --- Helper Function

// Set the Z, N flags for the Data Processing Instruction. The C flag is set in 
// the opDP__ functions for arithmetic operations, or stays as the result from 
// the barrel shifter in the case of operations.
void setFlagsDP(uint32_t value, struct processor arm) {
  //Set the Z flag
  bool allZero = value == 0;
  arm.registers[CPSR] = setBit(arm.registers[CPSR], allZero, Zbit);
  
  //Set the N flag
  uint32_t bit31 = value << 31;
  bool bit31set = (value & bit31) == bit31;
  arm.registers[CPSR] = setBit(arm.registers[CPSR], bit31set, Nbit);
}

// ====================== Helper Functions ====================================

// Returns a given word except with a single bit set in the given position
uint32_t setBit(uint32_t word, bool set, uint8_t position) {
  //Clears the bit at the given position and then sets it
  return (word & ~(1 << position)) | (set << position);
}
