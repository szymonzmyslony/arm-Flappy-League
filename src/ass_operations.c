#include "helperFunctions.h"
#include "ass_operations.h"
#include "emulate.h"
#include "ass_symbols.h"
#include "assemble.h"

#define ENC_BRANCHING(name, condition)             \
uint32_t name(char **opFields){ 				   \			   
  uint32_t binInstruction = 0xA000000;           \	                                              
  binInstruction |= condition << 28;             \	
  uint32_t current =  getMemAddr(); \
  int opFieldsIndex = 0; \
  char *operand = opFields[opFieldsIndex]; \
  uint32_t labelAdress = getValFromStruct(&symbolsTable, operand); 	 \
  current=current+8;  \
  int offset = labelAdress-current;	  \	
  offset=offset>>2;  \   
  binInstruction |= offset; 
  return binInstruction;                         \
}

ENC_BRANCHING(encodeBeq, COND_eq );

ENC_BRANCHING(encodeBne, COND_ne );

ENC_BRANCHING(encodeBge, COND_ge );

ENC_BRANCHING(encodeBlt, COND_lt);

ENC_BRANCHING(encodeBgt, COND_gt);

ENC_BRANCHING(encodeBle, COND_le);

ENC_BRANCHING(encodeBal, COND_al);

uint32_t encodeMul (char **opfields) {
  // Cond is 1110, A is 0 and S is 0;
  uint32_t binInstruction = 0xC0000090;
  int opFieldsIndex = 0;
  char *operand = opFields[opFieldsIndex];  
  //Set Rd  
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex << 12;
  binInstruction |= regIndex32;
  opFieldsIndex++;  
  //Set Rm  
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex;
  binInstruction |= regIndex32;
  opFieldsIndex++;  
  //set Rs 
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex<<4; 
  binInstruction |= regIndex32;  
}

uint32_t encodeMla (char **opfields) {
  // Cond is 1110, A is 1 and S is 0;
  uint32_t binInstruction = 0xC0200090;
  int opFieldsIndex = 0;
  char *operand = opFields[opFieldsIndex];  
  //Set Rd
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex << 12;
  binInstruction |= regIndex32;
  opFieldsIndex++;   
  //Set Rm 
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex;
  binInstruction |= regIndex32;
  opFieldsIndex++;  
  //set Rs 
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex<<4; 
  binInstruction |= regIndex32;
  opFieldsIndex++;   
  //set Rn 
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex<<8; 
  binInstruction |= regIndex32;
   
   
  
}


#include "emulate.h"
#include "ass_operations.h"

uint32_t encodeSDTldr (char **opfields, char *filename) {
  // initialise binInstruction and set cond to always (1110) and load bit set
  uint32_t binInstruction = 0xe4100000;
  int opFieldsIndex = 0;

  // resolve all opFields
  char *operand = opFields[opFieldsIndex];
  removeLeadingSpace(operand);

  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex << 12;
  binInstruction |= regIndex32;

  opFieldsIndex++;
  operand = opFields[opFieldsIndex]; 
  removeLeadingSpace(operand);

  // if loading a numerical constant - ldr only
  if (operand[0] == '=') {
    operand++;
    char *ptr;
    long int num = expToL(operand, ptr);
    uint32_t num32 = num;
    if (num < 0xFF) {
      char *newExpression = malloc(strlen(operand) + 2);
      if (newExpression == NULL) {
        fprintf(stderr, "Memory allocation failure");
        exit(EXIT_FAILURE);
      }
      strcpy(newExpression, "#");
      strcat(newExpression, operand);
      binInstruction = encodeDPmov(opFields[0], newExpression);
    } else {
      binInstruction |= (num & MASK11_0);
      binInstruction = setBit(binInstruction, true, pFlag);
      binInstruction = setBit(binInstruction, true, uFlag);
      uint32_t currMemAddress = getMemAddress();
      uint32_t offset = endOfFileAddr - currMemAddress;
      // account for pipeline while encoding offset, at time of execution
      //   PC will be 8 bytes ahead
      offset -= 8;

      if (offset > 0xFFFFFFFFFFFF){
        fprintf(stderr, "Error, cannot store constant at end of file");
      } else {
        // set nReg to PC
        binInstruction |= (PC << 16); 
        // set offset
        binInstruction |= offset;
      }
      uint32_t num32LE = switchEndy32(num32);
      appendBytes(char *filename, (char *) &num32LE, 4);
    }
    operand--;
  }
  
  // if loading an indexed address
  if (operand[0] == '[') {
    operand++;
    char *ptr;
    long int regIndex = getRegIndex(operand);
    uint32_t regIndex32 = (regIndex << 16);
    binInstruction |= regIndex32;
    operand = movToLastChar(operand);
    
    if (operand[0] == ']') {
      if (opFieldIndex == sizeof(opFields)) {
        // pre-indexing with no offset
        binInstruction = setBit(binInstruction, true, pBit);
        binInstruction = setBit(binInstruction, true, uBit);
      } else {
        // post-indexing
        if (operand[0] == '+') {
          binInstruction = setBit(binInstruction, true, uBit);
          operand++;
        } else if (operand[0] == '-') {
          operand++;
        }
        if (operand[0] == '#') {
          // offset is a number
          char *ptr;
          long int num = expToL(operand, ptr);
          binInstruction |= (num & MASK11_0);
        } else {
          // offset is a (shifted) register
          binInstruction = setBit(binInstruction, true, iBit);
          long int mRegIndex = getRegIndex(operand);
          binInstruction |= mRegIndex;
          if (opFieldIndex != sizeof(opFields)) {
            // shift
            binInstruction = setBit(binInstruction, true, 4);
            opFieldsIndex++;
            operand = opFields[FieldsIndex];
            removeLeadingSpace(operand);
            uint8_t shiftCode = getShiftCode(operand);
            binInstruction |= (shiftCode << 5);
            removeLeadingSpace(operand);
            long int sRegIndex = getRegIndex(operand);
            binInstruction |= (sRegIndex << 8);
          }
        }
      }
    } else {
      // pre-indexing with offset
      binInstruction = setBit(binInstruction, true, pBit);
      opFieldsIndex++;
      operand = opFields[opFieldsIndex]
      removeLeadingSpace(operand);
      }
      if (operand[0] == '+') {
        binInstruction = setBit(binInstruction, true, uBit);
        operand++;
      } else if (operand[0] == '-') {
        operand++;
      }
      if (operand[0] == '#') {
        // offset is a number
        char *ptr;
        long int num = expToL(operand, ptr);
        binInstruction |= (num & MASK11_0);
      } else {
        // offset is a (shifted) register
        binInstruction = setBit(binInstruction, true, iBit);
        char *ptr;
        long int mRegIndex = getRegIndex(operand);
        binInstruction |= mRegIndex;
        if (opFieldIndex != sizeof(opFields)) {
          // shift
          binInstruction = setBit(binInstruction, true, 4);
          opFieldsIndex++;
          operand = opFields[FieldsIndex];
          removeLeadingSpace(operand);
          uint8_t shiftCode = getShiftCode(operand);
          binInstruction |= (shiftCode << 5);
          removeLeadingSpace(operand);
          char *ptr;
          long int sRegIndex = strtol(operand, &ptr, base);
          binInstruction |= (sRegIndex << 8);
        }
      }
    }
  }

  // add value to end of file if necessary

  // return
  return binInstruction;
} 

// gets shiftCode and moves pointer to end of shift opCode
uint8_t getShiftCode(char *str) {
  if (str == 'l') {
    str++;
    if (str == 's') {
      str++;
      if (str == 'l') {
        str++;
        return 0;
      } else if (str == 'r'){
        str++;
        return 1;
      }
    }
  } else if (str == 'a') {
    str++;
    if (str == 's') {
      str++;
      if (str == 'r') {
        str++;
        return 2;
      }
    }
  } else if (str == 'r') {
    str++;
    if (str == 'o') {
      str++;
      if (str == 'r') {
        str++;
        return 3;
      }
    }
  }
  fprintf(stderr, "Invalid shiftCode");
  return 0;
}

// moves a pointer to a string past the white spaces
void removeLeadingSpace(char *str){
  while (*str == ' ') {
    str++;
  }
}
// takes an expression of the form #1234 and returns the long in
// ptr is set to the end of the int
long int expToL(char *expression, char *ptr) {
  int base = 10;
  if (strlen(operand) > 2) {
    if (operand[1] == 'x'){
      base = 16;
      operand += 2;
    }
  }
  return strtol(operand, &ptr, base);
}

uint32_t encodeSDTstr(char **opfields) {
  // initialise binInstruction and set cond to always (1110)
  uint32_t binInstruction = encodeSDTldr(opfields);
  binInstruction = setBit(binInstrcution, false, lBit);
  // check that instruction is not storing a numerical constant
  if (removeLeadingSpace(opfields[1]) = '='){
    fprintf(stderr, "Invalid Instruction: Cannot strore into a numerical \
            constant");
    return 0;
  }
  return binInstruction;
} 

uint32_t encodelsl(char **opfields) {
  char *shiftOpfield = malloc(strlen("lsl ") + strlen(opfields[1]) + 1);
  if (shiftOpfield == NULL) {
    fprintf(stderr, "Error, Memory allocation failed");
    exit(EXIT_FAILURE);
  }
  strcpy(shiftOpfield, "lsl ");
  strcat(shiftOpfield, opfields[1]);
  char **newOpfields = {opfields[0], opfields[0], shiftOpfield};
  uint32_t binInstruction = encodeDPmov(newOpfields);
  return binInstruction;
}

