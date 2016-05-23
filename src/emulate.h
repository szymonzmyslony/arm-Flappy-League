#ifndef EMULATE_H
#define EMULATE_H

// Structs
struct processor;
struct arguments;

// Main flow
void execute(struct arguments *decodedArgs, struct processor *arm);
void decode(uint32_t dInstruction, struct arguments *decodedArgs);
uint32_t fetch(struct processor arm);
void print(uint32_t arr[], uint32_t length);
void loadFile(char name[], struct processor *pointer);



// Data Processing

// Branching
void decodeDP(int dInstruction, struct arguments *decodedArgs);
void execBranching(struct arguments decodedArgs, struct processor *	arm);


// Multiply

// Single Data Transfter

// Helper functions
bool getBit(uint32_t word, uint8_t position);
uint32_t setBit(uint32_t word, bool set, uint8_t position);

void resolveOperand2(uint16_t op, bool iFlag, struct arguments *decodedArgs,
        struct processor *arm);
uint32_t rotateRight32(uint32_t val, uint16_t n);
uint32_t arithShiftRight32(uint32_t val, uint16_t n);
uint32_t shift(uint8_t shiftCode, uint32_t value, uint16_t,
        struct processor *arm, bool sFlag);

#endif
