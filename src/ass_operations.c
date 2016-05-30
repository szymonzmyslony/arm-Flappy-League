#include "helperFunctions.h"
#include "emulate.h"
#include "ass_operations.h"

uint32_t encodeSDTldr (char **opfields) {
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
    if (num < 0xFF) {
      binInstruction = 0; //TODO: call encode function for mov
    } else {
      binInstruction |= (num & MASK11_0);
      binInstruction = setBit(binInstruction, true, pFlag);
      binInstruction = setBit(binInstruction, true, uFlag);
    }
    operand--;
  }
  
  // if loading an indexed address
  if (operand[0] == '[') {
    operand++;
    char *ptr;
    long int regIndex = strtol(operand, &ptr, base);
    uint32_t regIndex32 = (regIndex << 16);
    binInstruction |= regIndex32;
    operand = ptr;

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
          char *ptr;
          long int mRegIndex = strtol(operand, &ptr, base);
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
        long int mRegIndex = strtol(operand, &ptr, base);
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
