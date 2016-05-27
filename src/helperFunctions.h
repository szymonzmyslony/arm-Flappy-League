#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H
#include "emulate.h"



// Helper functions

uint32_t switchEndy32(uint32_t value);
bool getBit(uint32_t word, uint8_t position);
uint32_t setBit(uint32_t word, bool set, uint8_t position);
uint32_t rotateRight32(uint32_t val, uint16_t n);
uint32_t arithShiftRight32(uint32_t val, uint16_t n);

#endif
