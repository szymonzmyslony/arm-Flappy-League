#ifndef EMULATE_SDT_H
#define EMULATE_SDT_H

#include "emulate_Error.h"

// Single Data Transfer
void decodeSDT(uint32_t dInstruction, arguments *decodedArgs);
void execSDT(arguments *decodedArgs, processor *arm);
void ldrSDTpre(arguments *decodedArgs, processor *arm);
void ldrSDTpost(arguments *decodedArgs, processor *arm);
void strSDTpre(arguments *decodedArgs, processor *arm);
void strSDTpost(arguments *decodedArgs, processor *arm);

#endif
