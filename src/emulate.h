#ifndef EMULATE_H
#define EMULATE_H

// Structs
struct processor;
struct arguments;

// Main flow
uint32_t fetch(struct processor arm);
void decode(uint32_t dInstruction, struct arguments *decodedArgs);

// Data Processing

// Branching

// Multiply

// Single Data Transfter

// Helper functions
uint32_t setBit(uint32_t word, bool set, uint8_t position);

void resolveOperand2(uint16_t op, bool iFlag, struct arguments *decodedArgs,
        struct processor *arm);
uint32_t rotateRight32(uint32_t val, uint16_t n);
uint32_t arithShiftRight32(uint32_t val, uint16_t n);
uint32_t shift(uint8_t shiftCode, uint32_t value, uint16_t,
        struct processor *arm, bool sFlag);

#endif
