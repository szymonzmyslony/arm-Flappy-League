#ifndef EMULATE_H
#define EMULATE_H

uint32_t setBit(uint32_t word, bool set, uint8_t position);
uint32_t rotateRight32(uint32_t val, uint16_t n);
uint32_t arithShiftRight32(uint32_t val, uint16_t n);
uint32_t shift(uint8_t shiftCode, uint32_t value, uint16_t n);
uint32_t fetch(struct processor arm);

#endif
