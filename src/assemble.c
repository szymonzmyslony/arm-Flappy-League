#include "assemble.h"
#define NUM_OF_FUNCTIONS   23 

// The byte address of the end of the binary file to be written
uint32_t endOfFileAddr = 0;
// The byte address that the line being processed is associated with
static uint32_t memAddr = 0;
// Pointer to file name
static char* fileName;

int main(int argc, char **argv) {
  fileName = argv[1];

  char *file; //TODO free this
  file = readfile(fileName); //TODO Define this. Should return a string.

  // The current line being processed. It is null-terminated when the line has
  // been processed.
  // We may assume that no line is longer than 511 characters
  // TODO check if it's ok for this to be on the stack
  char *line = calloc(MAX_LINE_LENGTH + 1, sizeof(char));
  checkAllocError(line, (MAX_LINE_LENGTH + 1) * sizeof(char));

  list operandTable;
  initialiseList(&operandTable);
  initOperandTable(&operandTable);

  list symbolsTable;
  initialiseList(symbolsTable);
  // Add all the opcodes' associated functions
  insertFront(operandTable, "add"  , (uint64_t) &encodeDPadd);
  insertFront(operandTable, "sub"  , (uint64_t) &encodeDPsub);
  insertFront(operandTable, "rsb"  , (uint64_t) &encodeDPrsb);
  insertFront(operandTable, "and"  , (uint64_t) &encodeDPand);
  insertFront(operandTable, "eor"  , (uint64_t) &encodeDPeor);
  insertFront(operandTable, "orr"  , (uint64_t) &encodeDPorr);
  insertFront(operandTable, "mov"  , (uint64_t) &encodeDPmov);
  insertFront(operandTable, "tst"  , (uint64_t) &encodeDPtst);
  insertFront(operandTable, "teq"  , (uint64_t) &encodeDPteq);
  insertFront(operandTable, "cmp"  , (uint64_t) &encodeDPcmp);
  insertFront(operandTable, "mul"  , (uint64_t) &encodeMul);
  insertFront(operandTable, "mla"  , (uint64_t) &encodeMla);
  insertFront(operandTable, "ldr"  , (uint64_t) &encodeSDTldr);
  insertFront(operandTable, "str"  , (uint64_t) &encodeSDTstr);
  insertFront(operandTable, "beq"  , (uint64_t) &encodeBeq);
  insertFront(operandTable, "bne"  , (uint64_t) &encodeBne);
  insertFront(operandTable, "bge"  , (uint64_t) &encodeBge);
  insertFront(operandTable, "blt"  , (uint64_t) &encodeBlt);
  insertFront(operandTable, "bgt"  , (uint64_t) &encodeBgt);
  insertFront(operandTable, "ble"  , (uint64_t) &encodeBle);
  insertFront(operandTable, "b"    , (uint64_t) &encodeBal);
  insertFront(operandTable, "lsl"  , (uint64_t) &encodelsl);
  insertFront(operandTable, "andeq", (uint64_t) &encodeDPandeq);

  // -- First pass : Find labels and associate them with a memory address
  // Let one instruction and its preceding label (if it exists) be a block
  // 'line' will only be used to record the label in this pass

  // The index of the current character to write in the line.
  int cIndex = 0;
  memAddr = 0;

  // Tracking the content of the current processed block - if we have found
  // a label and if we have found an instruction.
  bool foundLabel;
  bool foundInstruction;

  for (int i = 0; ; i++) {
    if(file[i] == '\n' || !file[i]) {
      // End of block has been found
      if(foundInstruction) {
        // Block had a label, process it
        if(foundLabel) {
          // Add mapping of (label, memAddr) to symbol table.
          insertFront(symbolsTable, trim(line), memAddr);

          foundLabel = false;
        }

        // Move on to the next block
        cIndex = 0;
        memAddr += 4;
        foundInstruction = false;
      }
      // No block, continue

      // Found a label, assume it is the first label in this block
    } else if(file[i] == ':') {
      assert(!foundLabel);
      foundLabel = true;
      // The text in the scanned block so far was not an instruction
      foundInstruction = false;
      // Line is now the label name, with leading/trailing whitespace
      line[cIndex] = "\0";

    } else {
      // Write the label to line
      if (!foundLabel) {
        line[cIndex] = file[i];
        cIndex++;
      }

      // A non-whitespace is either a label or an instruction.
      // If it is a label, we'll set it to false again once we find a colon
      if (!isspace(file[i])) {
        foundInstruction = true;
      }
    }

    if(!file[i]) {
      break;
    }
  }

  // TODO: edit this, doesn't allow for appending to file
  endOfFileAddr = memAddr;

  // -- Second pass : Find instructions and encode them, writing them into
  //                  a file
  // 'line' will only be used to record the instructions in this pass

  cIndex = 0;
  memAddr = 0;
  foundInstruction = false;

  for (int i = 0; ; i++) {
    if(file[i] == '\n' || !file[i]) {
      // Block found, process it
      if(foundInstruction) {
        // Line is the instruction, with leading & trailing whitespace
        line[cIndex] = "\0";

        // Parse the instruction
        char *opCode[MAX_OPCODE_LENGTH];
        char opFields[MAX_OPFIELD_SIZE][MAX_OPFIELD_LENGTH];
        tokenise (line, opCode, opFields);

        uint32_t (*functionPointer)(char **opFields);
	// retrieve 64bit int from opTable and cast as function pointer
        functionPointer = (uint32_t (*)(char **)) 
                getValFromStruct(operandTable, opCode);
        uint32_t instruction = functionPointer(opFields);
        uint32_t instructionLE = switchEndy32(instruction);
        addBytesToFile(fileName, memAddress, (char *) &instructionLE, 4);
        // TODO: ask uno if this is actual code
        // getValOfKey(symbolsTable, opCode)(opFields);

        cIndex = 0;
        memAddr += 4;
        foundInstruction = false;
      }
      // No block, continue

      // Found a label, assume it is the first label in this block
    } else if(file[i] == ':') {
      // The text in the scanned block so far was not an instruction
      foundInstruction = false;
      cIndex = 0;

    } else {
      // As above, a non-whitespace is either a label or an instruction.
      // If it is a label, it will be set to false once we find a colon
      if (!isspace(file[i])) {
        foundInstruction = true;
      }

      if(foundInstruction) {
        line[cIndex] = file[i];
        cIndex++;
      }
    }

    if(!file[i]) {
      break;
    }
  }

  destroyList(operandTable);
  destroyList(symbolsTable);
  free(line);

  return EXIT_SUCCESS;
}

uint32_t getMemAddr(void) {
  return memAddr;
}

uint32_t getFileName(void) {
  return fileName;
}
