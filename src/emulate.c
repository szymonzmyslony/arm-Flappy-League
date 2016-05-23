#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
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
#define MASK11_7              0x00000f80
#define MASK11_0              0x00000fff
#define MASK7_0               0x000000ff
#define MASK6_5               0x00000060

// condition cases
#define COND_eq                0
#define COND_ne                1
#define COND_ge               10
#define COND_lt               11
#define COND_gt               12
#define COND_le               13
#define COND_al               14

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
  uint8_t opCode;
  uint32_t operand2;
  uint8_t cond;
  uint32_t offset;
  void (*executePointer)(struct arguments *args, struct processor *arm); 
  bool aFlag;
  bool sFlag;
  bool lFlag;
  bool iFlag;
  bool pFlag;
  bool uFlag;
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
      execute(&decodedArgs, &arm);
    }

    // decode instruction
    if (arm.counter >= 1) {
      decode(dInstruction, &decodedArgs);
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

void decode(uint32_t dInstruction, struct arguments *decodedArgs){
  // set cond parameter to condition code
  decodedArgs->cond = (dInstruction >> 28);
  
  // set sFlag
  decodedArgs->sFlag = (dInstruction & (1 << 20));
  
  // set mask to bit 27
  uint32_t mask = 0x08000000;
  if ((dInstruction & mask) != 0){
    // Decode Branch instruction
    
    return;
  }

  // set mask to bit 26
  mask = 0x04000000;
  if ((dInstruction & mask) != 0){
    // set sFlag to false so that flags register is untouched within decodeSDT
    // Decode Single Data Transfer
    decodeSDT(dInstruction, decodedArgs);
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

// Calls the function in executePointer if the condition in cond is passed
void execute(struct arguments *decodedArgs, struct processor *arm) {
  
  bool executeFlag;
  bool zSet = getBit(arm->registers[CPSR], Zbit);
  bool nSet = getBit(arm->registers[CPSR], Nbit);
  bool vSet = getBit(arm->registers[CPSR], Vbit);
  
  switch(decodedArgs->cond) {
    case COND_eq: executeFlag = zSet; break;
    case COND_ne: executeFlag = !zSet; break;
    case COND_ge: executeFlag = nSet == vSet; break;
    case COND_lt: executeFlag = nSet != vSet; break;
    case COND_gt: executeFlag = !zSet && (nSet == vSet); break;
    case COND_le: executeFlag = zSet || (nSet != vSet); break;
    case COND_al: executeFlag = true; break;
  }
  
  if(executeFlag) {
    decodedArgs->executePointer(decodedArgs, arm);
  }
}

// Returns the instruction in the byte order as shown in the specification
// and increments the program counter
uint32_t fetch(struct processor arm) {
  return   (arm.memory[arm.registers[PC] + 3] << 24) 
         + (arm.memory[arm.registers[PC] + 2] << 16)
         + (arm.memory[arm.registers[PC] + 1] <<  8)
         +  arm.memory[arm.registers[PC]];
}

void decodeSDT(uint32_t dInstruction, struct arguments *decodedArgs) {

  // set lFlag, works since sFlag is always set in decode main function
  decodedArgs->lFlag = decodedArgs->sFlag;
  // sFlag must then be set to false
  decodedArgs->sFlag = false;

  decodedArgs->pFlag = (dInstruction & (1 << 24));
  decodedArgs->uFlag = (dInstruction & (1 << 23));
  decodedArgs->iFlag = (dInstruction & (1 << 25));

  decodedArgs->offset = (dInstruction & MASK11_0);
  decodedArgs->nRegIndex = ((dInstruction & MASK19_16) >> 16);
  decodedArgs->dRegIndex = ((dInstruction & MASK15_12) >> 12);

  decodedArgs->executePointer = &execSDT;
}

void execSDT(struct arguments *decodedArgs, struct processor *arm) {
  // account for pipeline
  if (decodedArgs->nRegIndex == PC) {
    decodedArgs->nRegIndex += 8;
  }
  //set offset
  resolveSDTOffset(decodedArgs->offset, decodedArgs->iFlag, decodedArgs, arm);
  if (decodedArgs->lFlag && decodedArgs->pFlag){
    ldrSDTpre(decodedArgs, arm);
  } else if (decodedArgs->lFlag && ~(decodedArgs->pFlag)){
    ldrSDTpost(decodedArgs, arm);
  } else if (~(decodedArgs->lFlag) && decodedArgs->pFlag){
    strSDTpre(decodedArgs, arm);
  } else {
    strSDTpost(decodedArgs, arm);
  }
}

void ldrSDTpre(struct arguments *decodedArgs, struct processor *arm) {
  uint32_t memAddress;
  if (decodedArgs->uFlag) {
    memAddress = arm->registers[decodedArgs->nRegIndex] + decodedArgs->offset;
  } else {
    memAddress = arm->registers[decodedArgs->nRegIndex] - decodedArgs->offset;
  }
  uint32_t littleEndVal = getLittleFromMem32(memAddress, arm);
  arm->registers[decodedArgs->dRegIndex] = switchEndy32(littleEndVal);
}

void ldrSDTpost(struct arguments *decodedArgs, struct processor *arm) {
  assert(decodedArgs->mRegIndex == decodedArgs->nRegIndex);
  uint32_t memAddress = arm->registers[decodedArgs->nRegIndex];
  uint32_t littleEndVal = getLittleFromMem32(memAddress, arm);
  arm->registers[decodedArgs->dRegIndex] = switchEndy32(littleEndVal);
  if (decodedArgs->uFlag) {
    arm->registers[decodedArgs->nRegIndex] += decodedArgs->offset;
  } else {
    arm->registers[decodedArgs->nRegIndex] -= decodedArgs->offset;
  }
}

void strSDTpre(struct arguments *decodedArgs, struct processor *arm) {
  uint32_t memAddress;
  if (decodedArgs->uFlag) {
    memAddress = arm->registers[decodedArgs->nRegIndex] + decodedArgs->offset;
  } else {
    memAddress = arm->registers[decodedArgs->nRegIndex] - decodedArgs->offset;
  }
  storeBigEndy32((arm->registers[decodedArgs->dRegIndex]),
          memAddress, arm);
}

void strSDTpost(struct arguments *decodedArgs, struct processor *arm) {
  assert(decodedArgs->mRegIndex == decodedArgs->nRegIndex);
  uint32_t memAddress = arm->registers[decodedArgs->nRegIndex];
  storeBigEndy32((arm->registers[decodedArgs->dRegIndex]),
          memAddress, arm);
  if (decodedArgs->uFlag) {
    arm->registers[decodedArgs->nRegIndex] += decodedArgs->offset;
  } else {
    arm->registers[decodedArgs->nRegIndex] -= decodedArgs->offset;
  }
}
// ====================== Helper Functions ====================================

// Gets a 32bit value from memory in little endian fashion
uint32_t getLittleFromMem32(uint32_t address, struct processor *arm) {
  uint32_t ret = arm->memory[address + 3] + (arm->memory[address + 2] << 8)
          + (arm->memory[address + 1] << 16) + (arm->memory[address] << 24);
  return ret;
} 

// Stores a big endian 32bit value in memory in a little endian fashion
void storeBigEndy32(uint32_t value, uint32_t address, struct processor *arm) {
  printf("\n%x\n", value);
  arm->memory[address] = value;
  arm->memory[address + 1] = value >> 8;
  arm->memory[address + 2] = value >> 16;
  arm->memory[address + 3] = value >> 24;
}

// Converts a 32bit unsigned int from little endian to big endian or reverse
uint32_t switchEndy32(uint32_t value) {
  uint8_t byte0 = value;
  uint8_t byte1 = value >> 8;
  uint8_t byte2 = value >> 16;
  uint8_t byte3 = value >> 24;
  return (byte3 + (byte2 << 8) + (byte1 << 16) + (byte0 << 24));
}

// Returns the value of a bit in a given position in a word
bool getBit(uint32_t word, uint8_t position) {
  return (word & (1 << position)) != 0;
}

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
// also sets mRegIndex
// assumes functionality according to iFlag in Data Processing 
//     i.e. flag is true -> immediate, flag is false -> shifted register
void resolveOperand2(uint16_t op, bool iFlag,
        struct arguments *decodedArgs, struct processor *arm) {
  // if i = 1 then immediate value
  if (iFlag){
    uint32_t rotation = (((MASK11_8 & op) >> 8) * 2);
    decodedArgs->operand2 = rotateRight32((op & MASK7_0), rotation);
  // if i = 0 then shifted register
  } else {
    // get number of reg to be shifted
    decodedArgs->mRegIndex = (MASK3_0 & op);

    // if bit 4 = 0 then integer shift
    if ((0x00000010 & op) == 0){
      decodedArgs->operand2 = shift((MASK6_5 & op) >> 5, 
              arm->registers[decodedArgs->mRegIndex], 
              ((MASK11_7 & op) >> 7), arm, decodedArgs->sFlag);
    // if bit 4 = 1 then register shift
    } else {
      uint16_t rotateAmount = arm->registers[(MASK11_8 & op) >> 8];
      decodedArgs->operand2 = shift((MASK6_5 & op) >> 5,
              arm->registers[decodedArgs->mRegIndex], 
              rotateAmount, arm, decodedArgs->sFlag);
    }
  }
}

// Resolves offset into an integer to be used in execution
void resolveSDTOffset(uint16_t offset, bool iFlag,
        struct arguments *decodedArgs, struct processor *arm) {
  if (iFlag) {
    uint32_t tempOp = decodedArgs->operand2;
    resolveOperand2(offset, ~iFlag, decodedArgs, arm);
    decodedArgs->offset = decodedArgs->operand2;
    decodedArgs->operand2 = tempOp;
    // note that operand 2 is unaltered
  } else {
    decodedArgs->offset = offset;
  }
}

// shifts value according to shift code by n bits
// sets carry flag if sFlag provided is true
uint32_t shift(uint8_t shiftCode, uint32_t value, uint16_t n, 
        struct processor *arm, bool sFlag){
  bool carry;
  switch (shiftCode){
    case 0x00:
      // set carry bit
      carry =  ((((0x00000001 << (sizeof(uint32_t) - n)) & value) != 0) && 
              sFlag);
      arm->registers[CPSR] = setBit(arm->registers[CPSR], carry, 31);
      
      return (value << n);
    case 0x01:
      // set carry bit
      carry = ((((0x00000001 << (n - 1)) & value) != 0) && sFlag);
      arm->registers[CPSR] = setBit(arm->registers[CPSR], carry, 31);
      return (value >> n);
    case 0x02:
      // set carry bit
      carry = ((((0x00000001 << (n - 1)) & value) != 0) && sFlag);
      arm->registers[CPSR] = setBit(arm->registers[CPSR], carry, 31);
      return arithShiftRight32(value, n);
    case 0x03:
      // set carry bit
      carry = ((((0x00000001 << (n - 1)) & value) != 0) && sFlag);
      arm->registers[CPSR] = setBit(arm->registers[CPSR], carry, 31);
      return rotateRight32(value, n);
    default:
      fprintf(stderr, "Invalid shift code %d to shift %d by %d", shiftCode,
              value, n);
      return 0;
  }
}
