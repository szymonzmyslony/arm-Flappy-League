#ifndef EMULATE_H
#define EMULATE_H
struct processor;
struct arguments;

void decode(uint32_t dInstruction, struct arguments decodedArgs);
uint32_t setBit(uint32_t word, bool set, uint8_t position);
uint32_t rotateRight32(uint32_t val, uint16_t n);
uint32_t arithShiftRight32(uint32_t val, uint16_t n);
void resolveOperand2(uint16_t op, bool iFlag, struct arguments *decodedArgs,
        struct processor *arm);
uint32_t shift(uint8_t shiftCode, uint32_t value, uint16_t,
        struct processor *arm, bool sFlag);
uint32_t fetch(struct processor arm);

#endif
