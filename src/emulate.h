#ifndef EMULATE_H
#define EMULATE_H
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "emulate_structs.h"


// Main flow
void execute(arguments *decodedArgs, processor *arm);
void decode(uint32_t dInstruction, arguments *decodedArgs);
uint32_t fetch(processor *arm);
void initProcessor(processor *arm);
void printReg(uint32_t arr[], uint32_t length);
void printMem(uint8_t arr[], uint32_t length);
void loadFile(char name[], processor *pointer);
void printSingleRegister(char regName[], uint32_t num);
void printPaddedNum(uint32_t num);

// Data Processing
void executeDP(arguments *decodedArgs, processor *arm);
void decodeDP(int dInstruction, arguments *decodedArgs);

// Branching
void decodeBranching(int dInstruction, arguments *decodedArgs);
void execBranching(arguments *decodedArgs, processor *arm);
uint32_t signEx24to32(uint32_t val24);

// Multiply
void decodeMul(int dInstruction, arguments *decodedArgs);
void mul(arguments *decodedArgs, processor *arm);

// Single Data Transfer
void decodeSDT(uint32_t dInstruction, arguments *decodedArgs);
void execSDT(arguments *decodedArgs, processor *arm);
void ldrSDTpre(arguments *decodedArgs, processor *arm);
void ldrSDTpost(arguments *decodedArgs, processor *arm);
void strSDTpre(arguments *decodedArgs, processor *arm);
void strSDTpost(arguments *decodedArgs, processor *arm);

// Error functions
void checkAllocError(void* ptr, uint32_t numBytes);
void printFileNotFoundError(char fileName[]);
bool outOfBounds(uint32_t memAddress);
void printOOBError(uint32_t memAddress);


#endif
