#ifndef EMULATE_H
#define EMULATE_H
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include "emulate_Branch.h"
#include "emulate_DP.h"
#include "emulate_Error.h"
#include "emulate_Mul.h"
#include "emulate_SDT.h"

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

#endif
