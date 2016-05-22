#ifndef EMULATE_H
#define EMULATE_H

// Structs
struct processor;
struct arguments;

// Helper Functions
void setFlagsZN(uint32_t value, struct processor *arm);
uint32_t setBit(uint32_t word, bool set, uint8_t position);
uint32_t rotateRight32(uint32_t val, uint16_t n);
uint32_t arithShiftRight32(uint32_t val, uint16_t n);
uint32_t shift(uint8_t shiftCode, uint32_t value, uint16_t n);
uint32_t fetch(struct processor arm);

// Decode Functions
void executeDP(struct arguments *decodedArgs, struct processor *arm);
void decodeDP(int dInstruction, struct arguments *decodedArgs);

#endif
