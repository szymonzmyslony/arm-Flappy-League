#ifndef EMULATE_H
#define EMULATE_H

void decode(uint32_t dInstruction, struct arguments decodedArgs);
uint32_t setBit(uint32_t word, bool set, uint8_t position);

#endif
