
#include "helperFunctions.h"

// ====================== Helper Functions ====================================
// Converts a 32bit unsigned int from little endian to big endian or reverse
uint32_t switchEndy32(uint32_t value) {
  uint8_t byte0 = value;
  uint8_t byte1 = value >> 8;
  uint8_t byte2 = value >> 16;
  uint8_t byte3 = value >> 24;
  return (byte3 + (byte2 << 8) + (byte1 << 16) + (byte0 << 24));
}

// Returns the value of a bit in a given position in a word
bool getBit(uint32_t word, uint8_t position) {
  return (word & (1 << position)) != 0;
}

// Returns a given word except with a single bit set in the given position
uint32_t setBit(uint32_t word, bool set, uint8_t position) {
  //Clears the bit at the given position and then sets it
  return (word & ~(1 << position)) | (set << position);
}

// Rotates a 32 bit value, val right by n bits
uint32_t rotateRight32(uint32_t val, uint16_t n){
  return ((val >> n) | (val << (32 - n)));
}

// Arithmetically shift right a 32 bit value, val right by n bits
uint32_t arithShiftRight32(uint32_t val, uint16_t n){
  // if MSB is 0 then same as logical shift
  if ((val & 0x80000000) == 0){
    return (val >> n);
  } else {
  // if MSB is 1 then the n most sig bits must be set to 1
    uint32_t mask = ~((0x00000001 << (32 - (n + 1))) - 1);
    return (mask | (val >> n));
  }
}


