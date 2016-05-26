#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
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
#define numberofelements      24

// masks for multiply
#define Abit                  21
#define Sbit                  20
                  
// masks
#define MASK3_0               0x0000000F
#define MASK11_8              0x00000F00
#define MASK15_12             0x0000F000
#define MASK19_16             0x000F0000
#define MASK24_21             0x01E00000
#define MASK0_23              0x00ffffff
#define MASK11_7              0x00000f80
#define MASK11_0              0x00000fff
#define MASK7_0               0x000000ff
#define MASK6_5               0x00000060

// opcodes
#define OPCODE_and             0
#define OPCODE_eor             1
#define OPCODE_sub             2
#define OPCODE_rsb             3
#define OPCODE_add             4
#define OPCODE_tst             8
#define OPCODE_teq             9
#define OPCODE_cmp            10
#define OPCODE_orr            12
#define OPCODE_mov            13

// condition cases
#define COND_eq                0
#define COND_ne                1
#define COND_ge               10
#define COND_lt               11
#define COND_gt               12
#define COND_le               13
#define COND_al               14

//Output
#define REG_PADDING            4
#define DEC_PADDING           11

struct processor {
  uint32_t registers[NUMBER_OF_REGISTERS];
  uint8_t memory[BYTES_IN_MEMORY];
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
  initProcessor(&arm);
  loadFile(argv[1], &arm); //
  
  // points to appropriate execute function after decoding
  uint32_t dInstruction; // instruction to be decoded next
  bool end;

  while (!end) {
    
    // execute instruction
    if (arm.counter >= 2) {
      execute(&decodedArgs, &arm);
      if (dInstruction == 0) {
        end = true;
      }
      
    }

    // decode instruction
    if (arm.counter >= 1) {
      decode(dInstruction, &decodedArgs);
    }

    // fetch instruction
    dInstruction = fetch(&arm);
    
    // increment program counter 
    arm.registers[PC] += INSTRUCTION_BYTES;

    // Increment counter but avoid overflow of counter
    if (arm.counter < 3){
      arm.counter++;
    }
  }
  
  // print register states
  printReg(arm.registers, NUMBER_OF_REGISTERS);
  printMem(arm.memory, BYTES_IN_MEMORY);
  
  return EXIT_SUCCESS;
  
}

void loadFile(char name[],struct processor *pointer) {	  
  FILE *myFile;
  myFile = fopen(name, "rb");
  // getting file length
  fseek(myFile, 0L, SEEK_END);
  long int sizeOfFile = ftell(myFile);
  rewind(myFile);
  assert(sizeOfFile <= BYTES_IN_MEMORY);

  fread((pointer->memory), 1, BYTES_IN_MEMORY, myFile);
  fclose(myFile);
}

void decode(uint32_t dInstruction, struct arguments *decodedArgs){

  // set cond parameter to condition code
  decodedArgs->cond = (dInstruction >> 28);
  
  // set sFlag
  decodedArgs->sFlag = (dInstruction & (1 << 20));
  decodedArgs->iFlag = (dInstruction & (1 << 25));
  
  // set mask to bit 27
  uint32_t mask = 0x08000000;
  if ((dInstruction & mask) != 0) {
    // Decode Branch instruction
    decodeBranching(dInstruction, decodedArgs);
    return;
  }

  // set mask to bit 26
  mask = 0x04000000;
  if ((dInstruction & mask) != 0) {
    // set sFlag to false so that flags register is untouched within decodeSDT
    // Decode Single Data Transfer
    decodeSDT(dInstruction, decodedArgs);
    return;
  }

  // set mask to bit 25
  mask = 0x02000000;
  if ((dInstruction & mask) != 0) {
    // Decode Data Processing (I = 1)
    decodeDP(dInstruction, decodedArgs);
    return;
  }

  // set mask to bit 4
  mask = 0x00000010;
  if ((dInstruction & mask) == 0) {
    // Decode Data Processing (I = 0 Constant Shift)
    decodeDP(dInstruction, decodedArgs);
    return;
  }

  // set mask to bit 7
  mask = 0x00000080;
  if ((dInstruction & mask) != 0) {
    // Decode Multiply
    decodeMul(dInstruction, decodedArgs);
    return;
  } else {
    // Decode Data Processing (I = 0 Shift by Register)
    decodeDP(dInstruction, decodedArgs);
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
uint32_t fetch(struct processor *arm) {
  return   (arm->memory[arm->registers[PC] + 3] << 24) 
         + (arm->memory[arm->registers[PC] + 2] << 16)
         + (arm->memory[arm->registers[PC] + 1] <<  8)
         +  arm->memory[arm->registers[PC]];
}

// Initialise values to zero
void initProcessor(struct processor *arm){
  for (uint32_t i = 0; i < BYTES_IN_MEMORY; i++) {
    arm->memory[i] = 0;
  }
  for (uint32_t i = 0; i < NUMBER_OF_REGISTERS; i++) {
    arm->registers[i] = 0;
  }
  arm->counter = 0;
}

// prints the non-zero values in an array of length, length
void printMem(uint8_t arr[], uint32_t length){
  printf("Non-zero memory:\n");
  for(uint32_t i=0; i < length; i+=4){
    // checks if memory is non-zero
    if(arr[i] || arr[i + 1] || arr[i + 2] || arr[i + 3]) {
      printf("0x%08x: 0x%02x%02x%02x%02x\n", i, arr[i], arr[i + 1], arr[i + 2],
              arr[i + 3]);
    }
  }
}

  
void printReg(uint32_t arr[], uint32_t length) {
// function for printing length number of registers
  int i;

  printf("Registers:\n");
  //Print registers 0-12
  for(i = 0; i < 13; ++i) {
    char regName[REG_PADDING + 1];
    snprintf(regName, REG_PADDING + 1, "$%d      ", i);
    regName[REG_PADDING] = '\0';
    
    printSingleRegister(regName, arr[i]);
  }

  printSingleRegister("PC  ", arr[PC]);     
  printSingleRegister("CPSR", arr[CPSR]);
}

void printSingleRegister(char regName[], uint32_t num) {
  printf("%s:", regName);
  printPaddedNum(num);
  printf(" (0x%08x)\n", num);  
}

// Pads the left side of the number so that the whole string has NUM_PADDING
// characters, and then prints the padded number.
void printPaddedNum(uint32_t num) {
  char decChar[DEC_PADDING + 1];
  snprintf(decChar, DEC_PADDING + 1, "%d", num);
  char decDisplay[DEC_PADDING + 1];

  for(int i = 0; i < DEC_PADDING; i++) {
    uint32_t numSpaces = DEC_PADDING - strlen(decChar);
    decDisplay[i] = i < numSpaces ? ' ' : decChar[i - numSpaces];
  }
  decDisplay[DEC_PADDING] = '\0';
  
  // For some reason, the test cases provided do not take into account the 
  // '-' sign when padding large numbers. This code deals with that.
  char minusPad[2] = { '\0', '\0' };
  minusPad[0] = decDisplay[0] == '-'? ' ' : '\0';
  
  printf("%s%s", minusPad, decDisplay);
}

// ================ Single Data Transfer Functions ============================
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

// For executing SDT load intruction, pre-increment
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

// For executing SDT load intruction, post-increment
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

// For executing SDT store intruction, pre-increment
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

// For executing SDT store intruction, post-increment
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

// ====================== Multiply Functions ==================================
// Decode Multiply
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


// Execute Multiply
void mul(struct arguments *decodedArgs, struct processor *arm) {
  uint32_t res = arm->registers[decodedArgs->mRegIndex] * 
                 arm->registers[decodedArgs->sRegIndex];
  if (decodedArgs->aFlag == true) {
    res = res + arm->registers[decodedArgs->nRegIndex];
  }
  if (decodedArgs->sFlag == true) {
    setFlagsMul(res, arm);
  }
  arm->registers[decodedArgs->dRegIndex] = res;
}


// Set Flags
void setFlagsMul(uint32_t value, struct processor *arm) {
  //Set N flag
  uint32_t bit31 = 1 << 31;
  bool bit31set = (value & bit31) == bit31;
  arm->registers[CPSR] = setBit(arm->registers[CPSR], bit31set, Nbit);

  //Set Z flag
  bool allZero = value == 0;
  arm->registers[CPSR] = setBit(arm->registers[CPSR], allZero, Zbit);
}

// ====================== Branching Functions =================================
// decode branching 
void decodeBranching(int dInstruction, struct arguments *decodedArgs) {
  // - decode operation
  decodedArgs->executePointer = &execBranching;
  //decode offset (still unsgined)
  uint32_t mask = MASK0_23;
  decodedArgs->offset = (dInstruction & mask);
}
  
  
// execute branching
void execBranching(struct arguments *decodedArgs, struct processor *arm) {
  bool negative = ((decodedArgs->offset)>>(numberofelements-1));
  
  uint32_t trueoffset;
  trueoffset = ((decodedArgs->offset)<<2);
  if (negative) {
    trueoffset = ~(signEx24to32(trueoffset));
    trueoffset++; 
  }
  if (negative){
    arm->registers[PC] = ((arm->registers[PC])-(trueoffset));
  } else{
    arm->registers[PC] = ((arm->registers[PC])+(trueoffset));
  }
//
  arm->counter=0;
}

// Extends sign of 24 bit signed int
uint32_t signEx24to32(uint32_t val24){
  if (val24>>(numberofelements+1)){
    return (0xfc000000 | val24);
  } else {
    return val24;
  }
}

// ========================= Data Processing ==================================

// Updates decodedArgs's opCode, nRegIndex, dRegIndex and executePointer
void decodeDP(int dInstruction, struct arguments *decodedArgs) {
  // - decode operation
  uint32_t mask = MASK24_21;
  //Remove the trailing 0s from the extracted opcode.
  uint8_t opCode = (dInstruction & mask) >> 21;
  decodedArgs->opCode = opCode;

  // - load undecoded operand2 into arguments
  decodedArgs->operand2 = (dInstruction & MASK11_0);

  // - decode Rn
  decodedArgs->nRegIndex = (dInstruction & MASK19_16) >> 16;

  // - decode Rd
  decodedArgs->dRegIndex = (dInstruction & MASK15_12) >> 12;
  
  // Set next execute to Data Processing
  decodedArgs->executePointer = &executeDP;
}

void executeDP(struct arguments *decodedArgs, struct processor *arm) {
  // Resolve operand2 based on the I flag
  resolveOperand2(decodedArgs->operand2, decodedArgs->iFlag, decodedArgs, arm);
  
  // The contents of the source register and the evaluated operand2
  uint32_t nRegContents = arm->registers[decodedArgs->nRegIndex];
  uint32_t operand2 = decodedArgs->operand2;
  
  // The result to be loaded into Rd, unchanged if no value given
  uint32_t res = arm->registers[decodedArgs->dRegIndex];
  
  // A pointer that points to the result which the ZN flags will update with
  uint32_t *flagsRes = &res;
  
  switch(decodedArgs->opCode) {
    // Rd is to be set
    case OPCODE_and: res = nRegContents & operand2; break;
    case OPCODE_eor: res = nRegContents ^ operand2; break;
    case OPCODE_orr: res = nRegContents | operand2; break;
    
    case OPCODE_mov: res = operand2;                break;
    
    // Rd is to be set and the C flag is to be set
    case OPCODE_sub: res = nRegContents - operand2; 
                     if(decodedArgs->sFlag) {    
                       bool borrow = !(operand2 > nRegContents);
                       arm->registers[CPSR] = 
                         setBit(arm->registers[CPSR], borrow, Cbit);
                     }
      break;
      
    case OPCODE_rsb: res = operand2 - nRegContents;
                     if(decodedArgs->sFlag) {    
                       bool borrow = !(nRegContents > operand2);
                       arm->registers[CPSR] = 
                         setBit(arm->registers[CPSR], borrow, Cbit);
                     }
      break;
      
    case OPCODE_add: res = operand2 + nRegContents;
                     if(decodedArgs->sFlag) {    
                       bool overflow = res < nRegContents || res < operand2;
                       arm->registers[CPSR] = 
                         setBit(arm->registers[CPSR], overflow, Cbit);
                     }
      break;
    // Rd is not to be set
    default: if(!decodedArgs->sFlag) {
               break;
             }
             
             uint32_t testRes;
             flagsRes = &testRes;
             
             switch(decodedArgs->opCode) {
               case OPCODE_tst: testRes = nRegContents & operand2; break;
               case OPCODE_teq: testRes = nRegContents ^ operand2; break;
               case OPCODE_cmp: testRes = nRegContents - operand2;
                                if(decodedArgs->sFlag) {    
                                  bool borrow = !(operand2 > nRegContents);
                                  arm->registers[CPSR] = 
                                    setBit(arm->registers[CPSR], borrow, Cbit);
                                }
                                break;
               //No such other opcode!
               default: assert(false); break;
             }
  }
  
  //Set Rd
  arm->registers[decodedArgs->dRegIndex] = res;
  // In the CPSR register, sets the Z and N flags if S is set
  if(decodedArgs->sFlag) {
    setFlagsZN(*flagsRes, arm);
  }
}

// ====================== Helper Functions ====================================

// Set the Z, N flags for the Data Processing Instruction. The C flag is set in 
// the opDP__ functions for arithmetic operations, or stays as the result from 
// the barrel shifter in the case of logical operations.
void setFlagsZN(uint32_t value, struct processor *arm) {
  //Set the Z flag
  bool allZero = (value == 0);
  arm->registers[CPSR] = setBit(arm->registers[CPSR], allZero, Zbit);

  //Set the N flag
  uint32_t bit31 = 1 << 31;
  bool bit31set = ((value & bit31) == bit31);
  arm->registers[CPSR] = setBit(arm->registers[CPSR], bit31set, Nbit);
  
}

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
void resolveOperand2(uint32_t op, bool iFlag,
        struct arguments *decodedArgs, struct processor *arm) {
  // if i = 1 then immediate value
  if (iFlag){
    uint32_t rotation = ((MASK11_8 & op) >> 8);
    rotation = (rotation << 1);
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
      carry =  (((0x00000001 << (sizeof(uint32_t) - n)) & value) != 0);
      if (sFlag) {
        arm->registers[CPSR] = setBit(arm->registers[CPSR], carry, 31);
      }
      return (value << n);
    case 0x01:
      // set carry bit
      carry = (((0x00000001 << (n - 1)) & value) != 0);
      if (sFlag) {
        arm->registers[CPSR] = setBit(arm->registers[CPSR], carry, 31);
      }
      return (value >> n);
    case 0x02:
      // set carry bit
      carry = (((0x00000001 << (n - 1)) & value) != 0);
      if (sFlag) {
        arm->registers[CPSR] = setBit(arm->registers[CPSR], carry, 31);
      }
      return arithShiftRight32(value, n);
    case 0x03:
      // set carry bit
      carry = (((0x00000001 << (n - 1)) & value) != 0);
      if (sFlag) {
        arm->registers[CPSR] = setBit(arm->registers[CPSR], carry, 31);
      }
      return rotateRight32(value, n);
    default:
      fprintf(stderr, "Invalid shift code %d to shift %d by %d", shiftCode,
              value, n);
      return 0;
  }
}

