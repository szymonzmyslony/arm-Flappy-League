#include "helperFunctions.h"
#include "emulate.h"
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
