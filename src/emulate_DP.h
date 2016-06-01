#ifndef EMULATE_DP_H
#define EMULATE_DP_H

#include <assert.h>
#include "helperFunctions.h"

// Data Processing Functions
void executeDP(arguments *decodedArgs, processor *arm);
void decodeDP(int dInstruction, arguments *decodedArgs);

#endif
