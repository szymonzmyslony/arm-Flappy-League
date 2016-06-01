#include "emulate_Mul.h"

// ====================== Multiply Functions ==================================
// Decode Multiply
void decodeMul(int dInstruction, arguments *decodedArgs) {
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
void mul(arguments *decodedArgs, processor *arm) {
  uint32_t res = arm->registers[decodedArgs->mRegIndex] *
                 arm->registers[decodedArgs->sRegIndex];
  if (decodedArgs->aFlag == true) {
    res = res + arm->registers[decodedArgs->nRegIndex];
  }
  if (decodedArgs->sFlag == true) {
    setZNFlags(res, arm);
  }
  arm->registers[decodedArgs->dRegIndex] = res;
}
