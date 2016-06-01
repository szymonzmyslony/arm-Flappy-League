#ifndef EMULATE_BRANCH_H
#define EMULATE_BRANCH_H

#include "helperFunctions.h"

// Branching
void decodeBranching(int dInstruction, arguments *decodedArgs);
void execBranching(arguments *decodedArgs, processor *arm);

#endif
