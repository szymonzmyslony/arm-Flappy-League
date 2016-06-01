#ifndef EMULATE_ERROR_H
#define EMULATE_ERROR_H

#include "helperFunctions.h"

// Error functions
void checkAllocError(void* ptr, uint32_t numBytes);
void printFileNotFoundError(char fileName[]);
bool outOfBounds(uint32_t memAddress);
void printOOBError(uint32_t memAddress);

#endif
