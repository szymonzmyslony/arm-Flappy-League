#include "emulate.h"
#include "helperFunctions.h"

int main(int argc, char **argv) {

  processor arm;
  arguments decodedArgs;

  // load program from args and initialise processor and read from file
  initProcessor(&arm);
  loadFile(argv[1], &arm); //

  // points to appropriate execute function after decoding
  uint32_t dInstruction; // instruction to be decoded next
  bool end = false;

  while (!end) {

    // execute instruction
    if (arm.counter >= 2) {
      execute(&decodedArgs, &arm);
      if ((dInstruction == 0) && (arm.counter != 0)) {
        end = true;
      }

    }

    // decode instruction
    if (arm.counter >= 1) {
      decode(dInstruction, &decodedArgs);
    }

    // fetch instruction
    dInstruction = fetch(&arm);

    // increment program counter
    arm.registers[PC] += INSTRUCTION_BYTES;

    // Increment counter but avoid overflow of counter
    if (arm.counter < 3){
      arm.counter++;
    }
  }

  // print register states
  printReg(arm.registers, NUMBER_OF_REGISTERS);
  printMem(arm.memory, BYTES_IN_MEMORY);

  // free allocated memory
  if(arm.memory != NULL) {
    free(arm.memory);
  }
  if(arm.memory != NULL) {
    free(arm.registers);
  }

  return EXIT_SUCCESS;

}

void loadFile(char name[],processor *pointer) {
  FILE *myFile;
  myFile = fopen(name, "rb");
  if(myFile == NULL) {
    printFileNotFoundError(name);
  }

  // getting file length
  fseek(myFile, 0L, SEEK_END);
  long int sizeOfFile = ftell(myFile);
  rewind(myFile);
  assert(sizeOfFile <= BYTES_IN_MEMORY);

  fread((pointer->memory), 1, BYTES_IN_MEMORY, myFile);
  fclose(myFile);
}

void decode(uint32_t dInstruction, arguments *decodedArgs){

  // set cond parameter to condition code
  decodedArgs->cond = (dInstruction >> 28);

  // set sFlag
  decodedArgs->sFlag = (dInstruction & (1 << 20));
  decodedArgs->iFlag = (dInstruction & (1 << 25));

  // set mask to bit 27
  uint32_t mask = 0x08000000;
  if ((dInstruction & mask) != 0) {
    // Decode Branch instruction
    decodeBranching(dInstruction, decodedArgs);
    return;
  }

  // set mask to bit 26
  mask = 0x04000000;
  if ((dInstruction & mask) != 0) {
    // set sFlag to false so that flags register is untouched within decodeSDT
    // Decode Single Data Transfer
    decodeSDT(dInstruction, decodedArgs);
    return;
  }

  // set mask to bit 25
  mask = 0x02000000;
  if ((dInstruction & mask) != 0) {
    // Decode Data Processing (I = 1)
    decodeDP(dInstruction, decodedArgs);
    return;
  }

  // set mask to bit 4
  mask = 0x00000010;
  if ((dInstruction & mask) == 0) {
    // Decode Data Processing (I = 0 Constant Shift)
    decodeDP(dInstruction, decodedArgs);
    return;
  }

  // set mask to bit 7
  mask = 0x00000080;
  if ((dInstruction & mask) != 0) {
    // Decode Multiply
    decodeMul(dInstruction, decodedArgs);
    return;
  } else {
    // Decode Data Processing (I = 0 Shift by Register)
    decodeDP(dInstruction, decodedArgs);
    return;
  }
}

// Calls the function in executePointer if the condition in cond is passed
void execute(arguments *decodedArgs, processor *arm) {
  bool executeFlag;
  bool zSet = getBit(arm->registers[CPSR], Zbit);
  bool nSet = getBit(arm->registers[CPSR], Nbit);
  bool vSet = getBit(arm->registers[CPSR], Vbit);

  switch(decodedArgs->cond) {
    case COND_eq: executeFlag = zSet; break;
    case COND_ne: executeFlag = !zSet; break;
    case COND_ge: executeFlag = nSet == vSet; break;
    case COND_lt: executeFlag = nSet != vSet; break;
    case COND_gt: executeFlag = !zSet && (nSet == vSet); break;
    case COND_le: executeFlag = zSet || (nSet != vSet); break;
    case COND_al: executeFlag = true; break;
  }

  if(executeFlag) {
    decodedArgs->executePointer(decodedArgs, arm);
  }
}

// Returns the instruction in the byte order as shown in the specification
// and increments the program counter
uint32_t fetch(processor *arm) {
  return   (arm->memory[arm->registers[PC] + 3] << 24)
         + (arm->memory[arm->registers[PC] + 2] << 16)
         + (arm->memory[arm->registers[PC] + 1] <<  8)
         +  arm->memory[arm->registers[PC]];
}

// Initialise values to zero
void initProcessor(processor *arm){
  arm->memory = (uint8_t *)calloc(BYTES_IN_MEMORY, sizeof(uint8_t));
  checkAllocError(arm->memory, BYTES_IN_MEMORY * sizeof(uint8_t));

  arm->registers = (uint32_t *)calloc(NUMBER_OF_REGISTERS, sizeof(uint32_t));
  checkAllocError(arm->registers, NUMBER_OF_REGISTERS * sizeof(uint32_t));

  arm->counter = 0;
}

// prints the non-zero values in an array of length, length
void printMem(uint8_t arr[], uint32_t length){
  printf("Non-zero memory:\n");
  for(uint32_t i=0; i < length; i+=4){
    // checks if memory is non-zero
    if(arr[i] || arr[i + 1] || arr[i + 2] || arr[i + 3]) {
      printf("0x%08x: 0x%02x%02x%02x%02x\n", i, arr[i], arr[i + 1], arr[i + 2],
              arr[i + 3]);
    }
  }
}


void printReg(uint32_t arr[], uint32_t length) {
// function for printing length number of registers
  int i;

  printf("Registers:\n");
  //Print registers 0-12
  for(i = 0; i < 13; ++i) {
    char regName[REG_PADDING + 1];
    snprintf(regName, REG_PADDING + 1, "$%d      ", i);
    regName[REG_PADDING] = '\0';

    printSingleRegister(regName, arr[i]);
  }

  printSingleRegister("PC  ", arr[PC]);
  printSingleRegister("CPSR", arr[CPSR]);
}

void printSingleRegister(char regName[], uint32_t num) {
  printf("%s:", regName);
  printPaddedNum(num);
  printf(" (0x%08x)\n", num);
}

// Pads the left side of the number so that the whole string has NUM_PADDING
// characters, and then prints the padded number.
void printPaddedNum(uint32_t num) {
  char decChar[DEC_PADDING + 1];
  snprintf(decChar, DEC_PADDING + 1, "%d", num);
  char decDisplay[DEC_PADDING + 1];

  for(int i = 0; i < DEC_PADDING; i++) {
    uint32_t numSpaces = DEC_PADDING - strlen(decChar);
    decDisplay[i] = i < numSpaces ? ' ' : decChar[i - numSpaces];
  }
  decDisplay[DEC_PADDING] = '\0';

  // For some reason, the test cases provided do not take into account the
  // '-' sign when padding large numbers. This code deals with that.
  char minusPad[2] = { '\0', '\0' };
  minusPad[0] = decDisplay[0] == '-'? ' ' : '\0';

  printf("%s%s", minusPad, decDisplay);
}

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

  if(outOfBounds(memAddress)) {
    printOOBError(memAddress);
    return;
  }

  uint32_t littleEndVal = getLittleFromMem32(memAddress, arm);
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

  if(outOfBounds(memAddress)) {
    printOOBError(memAddress);
    return;
  }

  uint32_t littleEndVal = getLittleFromMem32(memAddress, arm);
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

  if(outOfBounds(memAddress)) {
    printOOBError(memAddress);
    return;
  }

  storeBigEndy32((arm->registers[decodedArgs->dRegIndex]),
          memAddress, arm);
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

  if(outOfBounds(memAddress)) {
    printOOBError(memAddress);
    return;
  }

  storeBigEndy32((arm->registers[decodedArgs->dRegIndex]),
          memAddress, arm);
  if (decodedArgs->uFlag) {
    arm->registers[decodedArgs->nRegIndex] += decodedArgs->offset;
  } else {
    arm->registers[decodedArgs->nRegIndex] -= decodedArgs->offset;
  }
}

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


// ====================== Branching Functions =================================
// decode branching
void decodeBranching(int dInstruction, arguments *decodedArgs) {
  // - decode operation
  decodedArgs->executePointer = &execBranching;
  //decode offset (still unsgined)
  uint32_t mask = MASK0_23;
  decodedArgs->offset = (dInstruction & mask);
}


// execute branching
void execBranching(arguments *decodedArgs, processor *arm) {
  bool negative = ((decodedArgs->offset)>>(numberofelements-1));

  uint32_t trueoffset;
  trueoffset = ((decodedArgs->offset)<<2);
  if (negative) {
    trueoffset = ~(signEx24to32(trueoffset));
    trueoffset++;
  }
  if (negative){
    arm->registers[PC] = ((arm->registers[PC])-(trueoffset));
  } else{
    arm->registers[PC] = ((arm->registers[PC])+(trueoffset));
  }
//
  arm->counter=0;
}

// Extends sign of 24 bit signed int
uint32_t signEx24to32(uint32_t val24){
  if (val24>>(numberofelements+1)){
    return (0xfc000000 | val24);
  } else {
    return val24;
  }
}

// ========================= Data Processing ==================================

// Updates decodedArgs's opCode, nRegIndex, dRegIndex and executePointer
void decodeDP(int dInstruction, arguments *decodedArgs) {
  // - decode operation
  uint32_t mask = MASK24_21;
  //Remove the trailing 0s from the extracted opcode.
  uint8_t opCode = (dInstruction & mask) >> 21;
  decodedArgs->opCode = opCode;

  // - load undecoded operand2 into arguments
  decodedArgs->operand2 = (dInstruction & MASK11_0);

  // - decode Rn
  decodedArgs->nRegIndex = (dInstruction & MASK19_16) >> 16;

  // - decode Rd
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
               //No such other opcode!
               default: assert(false); break;
             }
  }

  //Set Rd
  arm->registers[decodedArgs->dRegIndex] = res;
  // In the CPSR register, sets the Z and N flags if S is set
  if(decodedArgs->sFlag) {
    setZNFlags(*flagsRes, arm);
  }
}

// ====================== Error Functions =====================================

// Returns whether the memory address is out of bounds
bool outOfBounds(uint32_t memAddress) {
  return memAddress > BYTES_IN_MEMORY;
}

// Prints an error message for out of bounds memory access
void printOOBError(uint32_t memAddress) {
  fprintf(stderr, "Error: Out of bounds memory access at address 0x%08x\n",
     memAddress);
}

// Checks for a memory allocation error and prints an error message
void checkAllocError(void* ptr, uint32_t numBytes) {
  if(ptr == NULL) {
    fprintf(stderr, "Error: Memory allocation failed for %u bytes\n", numBytes);
  }
}

// Prints an error message for file not found
void printFileNotFoundError(char fileName[]) {
  fprintf(stderr, "Error: File \'%s\' not found\n", fileName);
}
