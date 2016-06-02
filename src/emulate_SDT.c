#include "emulate_SDT.h"

// ================ Single Data Transfer Functions ============================
void decodeSDT(uint32_t dInstruction, arguments *decodedArgs) {

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

void execSDT(arguments *decodedArgs, processor *arm) {
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
void ldrSDTpre(arguments *decodedArgs, processor *arm) {
  uint32_t memAddress;
  if (decodedArgs->uFlag) {
    memAddress = arm->registers[decodedArgs->nRegIndex] + decodedArgs->offset;
  } else {
    memAddress = arm->registers[decodedArgs->nRegIndex] - decodedArgs->offset;
  }

  uint32_t littleEndVal;

  if (checkGPIO(memAddress, false)) {
    littleEndVal = switchEndy32(memAddress); 
  } else {
    if(outOfBounds(memAddress)) {
      printOOBError(memAddress);
      return;
    }

    littleEndVal = getLittleFromMem32(memAddress, arm);
  }
  arm->registers[decodedArgs->dRegIndex] = switchEndy32(littleEndVal);
}

// For executing SDT load intruction, post-increment
void ldrSDTpost(arguments *decodedArgs, processor *arm) {
  uint32_t memAddress = arm->registers[decodedArgs->nRegIndex];

  if (decodedArgs->iFlag
          && (decodedArgs->mRegIndex == decodedArgs->nRegIndex)) {
    fprintf(stderr, "Error: Invalid registers for post-incrementing load to : \
            %08x", memAddress);
    return;
  }
  
  uint32_t littleEndVal;

  if (checkGPIO(memAddress, false)){
    littleEndVal = switchEndy32(memAddress); 
  } else {
    if(outOfBounds(memAddress)) {
      printOOBError(memAddress);
      return;
    }
    littleEndVal = getLittleFromMem32(memAddress, arm);
  }
  arm->registers[decodedArgs->dRegIndex] = switchEndy32(littleEndVal);
  if (decodedArgs->uFlag) {
    arm->registers[decodedArgs->nRegIndex] += decodedArgs->offset;
  } else {
    arm->registers[decodedArgs->nRegIndex] -= decodedArgs->offset;
  }
}

// For executing SDT store intruction, pre-increment
void strSDTpre(arguments *decodedArgs, processor *arm) {
  uint32_t memAddress;
  if (decodedArgs->uFlag) {
    memAddress = arm->registers[decodedArgs->nRegIndex] + decodedArgs->offset;
  } else {
    memAddress = arm->registers[decodedArgs->nRegIndex] - decodedArgs->offset;
  }
  
  uint32_t storeVal = (arm->registers[decodedArgs->dRegIndex]);

  if (!checkGPIO(memAddress, storeVal)) {
    if(outOfBounds(memAddress)) {
      printOOBError(memAddress);
      return;
    }

    storeBigEndy32(storeVal, memAddress, arm);
  }
}

// For executing SDT store intruction, post-increment
void strSDTpost(arguments *decodedArgs, processor *arm) {
  uint32_t memAddress = arm->registers[decodedArgs->nRegIndex];

  if (decodedArgs->iFlag
          && (decodedArgs->mRegIndex == decodedArgs->nRegIndex)) {
    fprintf(stderr, "Error: Invalid registers for post-incrementing store to : \
            %08x", memAddress);
    return;
  }

  uint32_t storeVal = (arm->registers[decodedArgs->dRegIndex]);

  if(!checkGPIO(memAddress, storeVal)){
    if(outOfBounds(memAddress)) {
      printOOBError(memAddress);
      return;
    }
    storeBigEndy32(storeVal, memAddress, arm);
  }

  if (decodedArgs->uFlag) {
    arm->registers[decodedArgs->nRegIndex] += decodedArgs->offset;
  } else {
    arm->registers[decodedArgs->nRegIndex] -= decodedArgs->offset;
  }
}
