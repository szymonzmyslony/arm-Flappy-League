#include "helperFunctions.h"
#include "emulate.h"
#include "ass_operations.h"
#include "ass_symbols.h"
#include "assemble.h"

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
	  char *junk;
	  long expr = expToL(op1, junk);

	  //Check if the expression can be in the form of [Rotate|Imm]
	  if(expr > ((2 << 32) - 1)) {
	    fprintf(stderr, "Immediate value is too large at: %s", getMemAddr());
	    assert(false);
	  }

	  bool bitFound = false;
	  bool firstBitOdd = false;
	  int bitDistance = 0;
		uint32_t imm = 0;
	  // Loop through cycled bit representation of the expression
	  for(int i = (2 * 32 - 1); i >= 0; i--) {
	    if(bitFound) {
	      bitDistance++;
	    }

			if(getBit(expr, i % 32)) {
	      if(!bitFound) {
          bitFound = true;
					firstBitOdd = i % 2 == 1;
	      }

	      if(bitDistance > 7 || (bitDistance > 6 && firstBitOdd)) {
					firstBitOdd = i % 2 == 1;
	        bitDistance = 0;
	      }

				imm = 1 << (7 - firstBitOdd - bitDistance);
	    }

	    if(bitDistance >= 31) {
	      break;
	    }
	  }
	  if(bitDistance < 31) {
	    fprintf(stderr, "Cannot represent the Imm value at: %s", getMemAddr());
      assert(false);
	  }

		// Immediate value is representable


	// Case Rm{, <shift>}
	} else {
		// Set Rm
		binInstruction |= getRegIndex(op1);

		char *shiftType = op2;
		shiftType[3] = '\0';
		char *shiftVal = op2 + 4;

		// Set Shift Type
		if(strcmp(shiftType, "lsl") == 0) {
		  // Do nothing, already 00
		} else if(strcmp(shiftType, "lsr") == 0) {
		  binInstruction |= 01 << 5;
		} else if(strcmp(shiftType, "asr") == 0) {
		  binInstruction |= 10 << 5;
		} else if(strcmp(shiftType, "ror") == 0){
		  binInstruction |= 11 << 5;
		} else {
		  assert(false);
		}

		binInstruction |= shiftType << 5;
		// Shift by constant
		if(shiftVal[0] == '#') {
		  char *junk;
	    long expr = expToL(shiftVal, junk);
	    assert(expr < 32);
	    binInstruction |= expr << 7;
		// Shift by register
		} else {
		  binInstruction |= 1 << 4;
		  binInstruction |= getRegIndex(shiftVal) << 8;
		}
	}

	return binInstruction;
}

uint32_t encodeDPandeq(char **opFields) {
  if(getRegIndex(opFields[0])
     == getRegIndex(opFields[1])
     == getRegIndex(opFields[2])
     == getRegIndex("r1")) {
    return 0;
  } else {
    assert(false);
    return 0;
  }
}




#define ENC_BRANCHING(name, condition)             \
uint32_t name(char **opFields){ 				   \
  uint32_t binInstruction = 0x0A000000;           \
  binInstruction |= condition << 28;             \
  uint32_t current =  getMemAddr(); \
  int opFieldsIndex = 0; \
  char *operand = opFields[opFieldsIndex]; \
  uint32_t labelAdress = getValFromStruct(getLabelTable(), operand); 	 \
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

uint32_t encodeSDTldr (char **opfields) {
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
