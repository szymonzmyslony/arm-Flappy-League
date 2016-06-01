#include "emulate.h"


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
