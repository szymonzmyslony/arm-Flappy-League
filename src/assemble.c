#include "assemble.h"

// The byte address of the end of the binary file to be written
uint32_t endOfFileAddr = 0;

int main(int argc, char **argv) {
  char *file;
  readfile(file); //TODO Define this. Should return a string.

  // The current line being processed. It is null-terminated when the line has
  // been processed.
  // We may assume that no line is longer than 511 characters
  // TODO check if it's ok for this to be on the stack
  char *line = calloc(MAX_LINE_LENGTH + 1, sizeof(char));
  checkAllocError(line, (MAX_LINE_LENGTH + 1) * sizeof(char));

  // -- First pass : Find labels and associate them with a memory address
  // Let one instruction and its preceding label (if it exists) be a block
  // 'line' will only be used to record the label in this pass

  // The index of the current character to write in the line.
  int cIndex = 0;
  // The byte address that the line being processed is associated with
  uint32_t memAddr = 0;

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
          // TODO Treat the whitespace of line.
          //      Add mapping of (line, memAddr) to symbol table.

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

        //TODO tokenise and encode, whitespace treated in tokenise

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

  return EXIT_SUCCESS;
}
