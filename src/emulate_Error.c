#include "emulate_Error.h"

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
