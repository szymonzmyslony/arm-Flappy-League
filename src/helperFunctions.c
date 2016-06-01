
#include "helperFunctions.h"

// ====================== Helper Functions which do not use structs============
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

// Returns the index as an int of a register given in the format r1, r2, etc
// Returns zero if index if string is not valid
// Works for only a maximum of 99 registers
uint8_t getRegIndex(char *str){
  if (strlen(str) < 2){
    fprintf(stderr, "Attempted read of invalid argument");
    return 0;
  }

  if (str[0] == 'r'){
    if (strlen(str) == 2) {
      return charToInt(str[1]);
    } else {
      if (str[2] == ',') {
        return charToInt(str[1]);
      } else {
        return ((charToInt(str[1]) * 10 ) + charToInt(str[2]));
      }
    }
  }

  fprintf(stderr, "Attempted read of invalid argument");
  return 0;
}

// Converts a char digit between '0' and '9' to an int
// returns 10 if digit is not valid
uint8_t charToInt(char c) {
  if (c < '0' || '9' < c) {
    fprintf(stderr, "Char c is not a valid digit");
    return 10;
  }
  uint8_t i = (uint8_t) c - (uint8_t) '0';
  return i;
}

// Moves a char ptr to the last character in the String
// If already at null char, do nothing
void movToLastChar(char *str) {
  if (str == '\0') {
    return;
  }
  while (str != '\0') {
    str++;
  }
  str--;
}
// Removes the leading and trailing whitespace in a string
void trim(char *string) {
  int start = 0;
  int end = strlen(string) - 1;

  while(isspace(string[start])) {
    start++;
  }

  while(isspace(string[end]) && start >= end) {
    end--;
  }

  for(int i = start; i <= end; i++) {
    string[i - start] = string[i];
  }

  string[end - start + 1] = '\0';

}

// ====================== Helper Functions which use structs===================

// Set the Z, N flags for the Data Processing Instruction. The C flag is set in
// the opDP__ functions for arithmetic operations, or stays as the result from
// the barrel shifter in the case of logical operations.
void setZNFlags(uint32_t value, processor *arm) {
  //Set the Z flag
  bool allZero = (value == 0);
  arm->registers[CPSR] = setBit(arm->registers[CPSR], allZero, Zbit);

  //Set the N flag
  uint32_t bit31 = 1 << 31;
  bool bit31set = ((value & bit31) == bit31);
  arm->registers[CPSR] = setBit(arm->registers[CPSR], bit31set, Nbit);
}

// Gets a 32bit value from memory in little endian fashion
uint32_t getLittleFromMem32(uint32_t address, processor *arm) {
  uint32_t ret = arm->memory[address + 3] + (arm->memory[address + 2] << 8)
          + (arm->memory[address + 1] << 16) + (arm->memory[address] << 24);
  return ret;
}

// Stores a big endian 32bit value in memory in a little endian fashion
void storeBigEndy32(uint32_t value, uint32_t address, processor *arm) {
  arm->memory[address] = value;
  arm->memory[address + 1] = value >> 8;
  arm->memory[address + 2] = value >> 16;
  arm->memory[address + 3] = value >> 24;
}

// Resolves operand2 into an integer to be used in execution
// Should only be called during execution
// also sets mRegIndex
// assumes functionality according to iFlag in Data Processing
//     i.e. flag is true -> immediate, flag is false -> shifted register
void resolveOperand2(uint32_t op, bool iFlag,
        arguments *decodedArgs, processor *arm) {
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
      uint16_t rotateAmount = ((arm->registers[(MASK11_8 & op) >> 8])
              & 0x000000FF);
      decodedArgs->operand2 = shift((MASK6_5 & op) >> 5,
              arm->registers[decodedArgs->mRegIndex],
              rotateAmount, arm, decodedArgs->sFlag);
    }
  }
}

// Resolves offset into an integer to be used in execution
void resolveSDTOffset(uint16_t offset, bool iFlag,
        arguments *decodedArgs, processor *arm) {
  if (iFlag) {
    uint32_t tempOp = decodedArgs->operand2;
    resolveOperand2(offset, 0, decodedArgs, arm);
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
        processor *arm, bool sFlag){
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
