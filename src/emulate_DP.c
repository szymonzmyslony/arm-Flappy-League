#include "emulate_DP.h"

// ========================= Data Processing ==================================

// Updates decodedArgs's opCode, nRegIndex, dRegIndex and executePointer
void decodeDP(int dInstruction, arguments *decodedArgs) {
  // Decode operation
  uint32_t mask = MASK24_21;
  // Remove the trailing 0s from the extracted opcode.
  uint8_t opCode = (dInstruction & mask) >> 21;
  decodedArgs->opCode = opCode;

  // Load undecoded operand2 into arguments
  decodedArgs->operand2 = (dInstruction & MASK11_0);

  // Decode Rn
  decodedArgs->nRegIndex = (dInstruction & MASK19_16) >> 16;

  // Decode Rd
  decodedArgs->dRegIndex = (dInstruction & MASK15_12) >> 12;

  // Set next execute to Data Processing
  decodedArgs->executePointer = &executeDP;
}

void executeDP(arguments *decodedArgs, processor *arm) {
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
               // No such other opcode!
               default: assert(false); break;
             }
  }

  // Set Rd
  arm->registers[decodedArgs->dRegIndex] = res;
  // In the CPSR register, sets the Z and N flags if S is set
  if(decodedArgs->sFlag) {
    setZNFlags(*flagsRes, arm);
  }
}
