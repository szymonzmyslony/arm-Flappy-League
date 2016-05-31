#include "helperFunctions.h"
#include "emulate.h"
#include "ass_operations.h"

#define DP_ENC_RES(name, opcode)                   \
uint32_t name(char **opFields){                    \
	uint32_t binInstruction = 0;                   \
	                                               \
	binInstruction |= COND_al << 28;               \
	                                               \
	binInstruction |= opcode << 21;                \
	                                               \
	uint32_t nRegIndex = getRegIndex(opFields[1]); \
	binInstruction |= nRegIndex << 16;             \
                                                   \
	uint32_t dRegIndex = getRegIndex(opFields[0]); \
	binInstruction |= dRegIndex << 12;             \
	                                               \
    binInstruction |= encodeOperand2(opFields, 2); \
                                                   \
	return binInstruction;                         \
}

#define DP_ENC_FLG(name, opcode)                   \
uint32_t name(char **opFields){                    \
	uint32_t binInstruction = 0;                   \
	                                               \
	binInstruction |= COND_al << 28;               \
	                                               \
	binInstruction |= opcode << 21;                \
	                                               \
    binInstruction |= 1 << Sbit;                   \
	                                               \
	uint32_t nRegIndex = getRegIndex(opFields[0]); \
	binInstruction |= nRegIndex << 16;             \
	                                               \
    binInstruction |= encodeOperand2(opFields, 1); \
                                                   \
	return binInstruction;                         \
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

	// Case <#expression>
	if(op1[0] == '#') {

	// Case Rm{, <shift>}
	} else {
		// Set Rm
		binInstruction |= getRegIndex(op1);

	}

	return binInstruction;
}
