#include "ass_operations.h"

#define DP_ENC_RES(name, opcode)                   \
uint32_t name(char **opFields){                    \
  uint32_t binInstruction = 0;                     \
                                                   \
  binInstruction |= COND_al << 28;                 \
                                                   \
  binInstruction |= opcode << 21;                  \
                                                   \
  uint32_t nRegIndex = getRegIndex(opFields[1]);   \
  binInstruction |= nRegIndex << 16;               \
                                                   \
  uint32_t dRegIndex = getRegIndex(opFields[0]);   \
  binInstruction |= dRegIndex << 12;               \
                                                   \
  binInstruction |= encodeOperand2(opFields, 2); \
                                                   \
  return binInstruction;                           \
}

#define DP_ENC_FLG(name, opcode)                   \
uint32_t name(char **opFields){                    \
  uint32_t binInstruction = 0;                     \
                                                   \
  binInstruction |= COND_al << 28;                 \
                                                   \
  binInstruction |= opcode << 21;                  \
                                                   \
  binInstruction |= 1 << Sbit;                   \
                                                   \
  uint32_t nRegIndex = getRegIndex(opFields[0]);   \
  binInstruction |= nRegIndex << 16;               \
                                                   \
  binInstruction |= encodeOperand2(opFields, 1); \
                                                   \
  return binInstruction;                           \
}

DP_ENC_RES(encodeDPand, OPCODE_and)
DP_ENC_RES(encodeDPeor, OPCODE_eor)
DP_ENC_RES(encodeDPsub, OPCODE_sub)
DP_ENC_RES(encodeDPrsb, OPCODE_rsb)
DP_ENC_RES(encodeDPadd, OPCODE_add)
DP_ENC_RES(encodeDPorr, OPCODE_orr)

DP_ENC_FLG(encodeDPtst, OPCODE_tst)
DP_ENC_FLG(encodeDPteq, OPCODE_teq)
DP_ENC_FLG(encodeDPcmp, OPCODE_cmp)

uint32_t encodeDPmov(char **opFields) {

  uint32_t binInstruction = 0;

  binInstruction |= COND_al << 28;

  binInstruction |= OPCODE_mov << 21;

  uint32_t dRegIndex = getRegIndex(opFields[0]);
  binInstruction |= dRegIndex << 12;

  binInstruction |= encodeOperand2(opFields, 1);
  return binInstruction;
}

// Takes an index, where opFields[index] is the first token in operand2
uint32_t encodeOperand2(char **opFields, uint8_t index) {
  uint32_t binInstruction = 0;

  char *op1 = opFields[index];
  char *op2 = opFields[index + 1];

  // Case <#expression>
  if(op1[0] == '#') {
    char *junk = NULL;
    long expr = expToL(op1, junk);

    //Check if the expression can be in the form of [Rotate|Imm]
    if((expr > (INT32_MAX)) || (expr < 0)) {
      fprintf(stderr, "Immediate value is too large at: %08x\n", getMemAddr());
      assert(false);
    }

    bool bitFound = false;
    int bitDistance = 0;
    uint32_t imm = 0;
    uint32_t rotate = 0;
    // Loop through cycled bit representation of the expression
    for(int i = 0; i < (2 * 32); i++) {
      if(bitFound) {
        bitDistance++;
      }
      
      if((expr & (1 << (i % 32))) != 0) {
        if(!bitFound) {
          bitFound = true;
          rotate = ((32 - ((i - 1) % 32)) % 32)/2;
          bitDistance = i % 2;
        }

        if(bitDistance > 7) {
          imm = 0;
          rotate = ((32 - ((i - 1) % 32)) % 32)/2;
          bitDistance = i % 2;
        }
        
        imm |= 1 << bitDistance;
      }

      if(bitDistance >= 31) {
        break;
      }
    }
    if(bitDistance < 31 && bitDistance != 0) {
      fprintf(stderr,"Cannot represent the Imm value at: %08x\n", getMemAddr());
      assert(false);
    }

    // Immediate value is representable
    binInstruction |= imm;
    binInstruction |= (rotate << 8);

    binInstruction |= setBit(binInstruction, true, Ibit);

  // Case Rm{, <shift>}
  } else {
    // Set Rm
    binInstruction |= getRegIndex(op1);
    char *shiftType = op2;
    shiftType[3] = '\0';
    char *shiftVal = op2 + 4;
  

    // only if a shift is provided
    if (shiftType[0] != '\0') {
      // Set Shift Type
      binInstruction |= ((getShiftCode(shiftType)) << 5);
  
      // Shift by constant
      if(shiftVal[0] == '#') {
        char *junk = NULL;
        long expr = expToL(shiftVal, junk);
        assert(expr < 32);
        binInstruction |= expr << 7;
      // Shift by register
      } else {
        binInstruction |= 1 << 4;
        binInstruction |= getRegIndex(shiftVal) << 8;
      }
    }
  }

  return binInstruction;
}

uint32_t encodeDPandeq(char **opFields) {
  if ((getRegIndex(opFields[0]) == getRegIndex(opFields[1]))
      && (getRegIndex(opFields[0]) == getRegIndex(opFields[2]))
      && (getRegIndex(opFields[0]) == getRegIndex("r0"))) {
    return 0;
  } else {
    assert(false);
    return 0;
  }
}


#define ENC_BRANCHING(name, condition)             \
uint32_t name(char **opFields){            \
  uint32_t binInstruction = 0x0A000000;           \
  binInstruction |= condition << 28;             \
  uint32_t current =  getMemAddr(); \
  int opFieldsIndex = 0; \
  char *operand = opFields[opFieldsIndex]; \
  uint32_t labelAdress = getValFromStruct(getLabelTable(), operand);   \
  current=current+8;  \
  int offset = labelAdress-current;   \
  offset=offset>>2;  \
  binInstruction |= offset;\
  return binInstruction;                         \
}

ENC_BRANCHING(encodeBeq, COND_eq )
ENC_BRANCHING(encodeBne, COND_ne )
ENC_BRANCHING(encodeBge, COND_ge )
ENC_BRANCHING(encodeBlt, COND_lt)
ENC_BRANCHING(encodeBgt, COND_gt)
ENC_BRANCHING(encodeBle, COND_le)
ENC_BRANCHING(encodeBal, COND_al)

uint32_t encodeMul (char **opFields) {
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
  regIndex = getRegIndex(operand);
  regIndex32 = regIndex;
  binInstruction |= regIndex32;
  opFieldsIndex++;
  //set Rs
  regIndex = getRegIndex(operand);
  regIndex32 = regIndex<<4;
  binInstruction |= regIndex32;
  return binInstruction;
}

uint32_t encodeMla (char **opFields) {
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
  regIndex = getRegIndex(operand);
  regIndex32 = regIndex;
  binInstruction |= regIndex32;
  opFieldsIndex++;
  //set Rs
  regIndex = getRegIndex(operand);
  regIndex32 = regIndex<<4;
  binInstruction |= regIndex32;
  opFieldsIndex++;
  //set Rn
  regIndex = getRegIndex(operand);
  regIndex32 = regIndex<<8;
  binInstruction |= regIndex32;
  return binInstruction;
}

uint32_t encodeSDTldr (char **opFields) {
  char *filename = getFileName();
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
    char *ptr = NULL;
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
      char *newOpfields[2];
      newOpfields[0] = opFields[0];
      newOpfields[1] = newExpression;
      binInstruction = encodeDPmov(newOpfields);
    } else {
      binInstruction |= (num & MASK11_0);
      binInstruction = setBit(binInstruction, true, Pbit);
      binInstruction = setBit(binInstruction, true, Ubit);

      uint32_t num32LE = switchEndy32(num32);
      uint32_t endOfFile = appendBytes(filename, (char *) &num32LE, 4);

      uint32_t currMemAddress = getMemAddr();
      uint32_t offset = endOfFile - currMemAddress;
      // account for pipeline while encoding offset, at time of execution
      //   PC will be 8 bytes ahead
      offset -= 8;

      if (offset > 0xFFFFFFFFFFFF){
        fprintf(stderr, "Error, cannot access constant at end of file");
      } else {
        // set nReg to PC
        binInstruction |= (PC << 16);
        // set offset
        binInstruction |= offset;
      }
    }
    operand--;
  }

  // if loading an indexed address
  if (operand[0] == '[') {
    operand++;
    long int regIndex = getRegIndex(operand);
    uint32_t regIndex32 = (regIndex << 16);
    binInstruction |= regIndex32;
    // move operand to last char
    while (operand[0] != '\0') {
      operand++;
    }
    operand--;

    if (operand[0] == ']') {
      opFieldsIndex++;
      operand = opFields[opFieldsIndex];
      if (operand[0] == '\0') {
        // pre-indexing with no offset
        binInstruction = setBit(binInstruction, true, Pbit);
        binInstruction = setBit(binInstruction, true, Ubit);
      } else {
        // post-indexing
        if (operand[0] == '+') {
          binInstruction = setBit(binInstruction, true, Ubit);
          operand++;
        } else if (operand[0] == '-') {
          operand++;
        }
        if (operand[0] == '#') {
          // offset is a number
          char *ptr = NULL;
          long int num = expToL(operand, ptr);
          binInstruction |= (num & MASK11_0);
        } else {
          // offset is a (shifted) register
          binInstruction = setBit(binInstruction, true, Ibit);
          long int mRegIndex = getRegIndex(operand);
          binInstruction |= mRegIndex;
          if (opFieldsIndex != sizeof(opFields)) {
            // shift
            binInstruction = setBit(binInstruction, true, 4);
            opFieldsIndex++;
            operand = opFields[opFieldsIndex];
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
      binInstruction = setBit(binInstruction, true, Pbit);
      opFieldsIndex++;
      operand = opFields[opFieldsIndex];
      removeLeadingSpace(operand);

      if (operand[0] == '+') {
        binInstruction = setBit(binInstruction, true, Ubit);
        operand++;
      } else if (operand[0] == '-') {
        operand++;
      }
      if (operand[0] == '#') {
        // offset is a number
        char *ptr = NULL;
        long int num = expToL(operand, ptr);
        binInstruction |= (num & MASK11_0);
      } else {
        // offset is a (shifted) register
        binInstruction = setBit(binInstruction, true, Ibit);
        long int mRegIndex = getRegIndex(operand);
        binInstruction |= mRegIndex;
        if (opFieldsIndex != sizeof(opFields)) {
          // shift
          binInstruction = setBit(binInstruction, true, 4);
          opFieldsIndex++;
          operand = opFields[opFieldsIndex];
          removeLeadingSpace(operand);
          uint8_t shiftCode = getShiftCode(operand);
          binInstruction |= (shiftCode << 5);
          removeLeadingSpace(operand);
          long int sRegIndex = getRegIndex(operand);
          binInstruction |= (sRegIndex << 8);
        }
      }
    }
  }

  return binInstruction;
}

// gets shiftCode and moves pointer to end of shift opCode
uint8_t getShiftCode(char *str) {
  if (str[0] == 'l') {
    str++;
    if (str[0] == 's') {
      str++;
      if (str[0] == 'l') {
        str++;
        return 0;
      } else if (str[0] == 'r'){
        str++;
        return 1;
      }
    }
  } else if (str[0] == 'a') {
    str++;
    if (str[0] == 's') {
      str++;
      if (str[0] == 'r') {
        str++;
        return 2;
      }
    }
  } else if (str[0] == 'r') {
    str++;
    if (str[0] == 'o') {
      str++;
      if (str[0] == 'r') {
        str++;
        return 3;
      }
    }
  }

  fprintf(stderr, "Invalid shiftCode %s\n", str);
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
  if(expression[0] == '#') {
    expression++;
  }
  if (strlen(expression) > 2) {
    if (expression[1] == 'x'){
      base = 16;
      expression += 2;
    }
  }
  return strtol(expression, &ptr, base);
}

uint32_t encodeSDTstr(char **opfields) {
  // initialise binInstruction and set cond to always (1110)
  uint32_t binInstruction = encodeSDTldr(opfields);
  binInstruction = setBit(binInstruction, false, Lbit);
  // check that instruction is not storing a numerical constant
  char *operand = opfields[1];
  removeLeadingSpace(operand);
  if (operand[0] == '='){
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
  char *newOpfields[3];
  newOpfields[0] = opfields[0];
  newOpfields[1] = opfields[0];
  newOpfields[2] = shiftOpfield;
  uint32_t binInstruction = encodeDPmov(newOpfields);
  return binInstruction;
}
