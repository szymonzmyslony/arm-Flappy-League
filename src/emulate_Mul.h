#ifndef EMULATE_MUL_H
#define EMULATE_MUL_H

#include "helperFunctions.h"

// Multiply
void decodeMul(int dInstruction, arguments *decodedArgs);
void mul(arguments *decodedArgs, processor *arm);

#endif
