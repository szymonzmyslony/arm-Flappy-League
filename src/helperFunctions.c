
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

// Returns the index as an int of a register given in the format r1, r2, etc
// Returns zero if index if string is not valid
// Works for only a maximum of 99 registers
uint8_t getRegIndex(char *str){
  if (strlen(str) < 2){
    fprintf(stderr, "Attempted read of invalid argument");
    return 0;
  }

  if (str[0] == 'r'){
    if (strlen(str) == 2) {
      return charToInt(str[1]);
    } else {
      if (str[2] == ',') {
        return charToInt(str[1]);
      } else {
        return ((charToInt(str[1]) * 10 ) + charToInt(str[2]));
      }
    }
  } 

  fprintf(stderr, "Attempted read of invalid argument");
  return 0;
}

// Converts a char digit between '0' and '9' to an int
// returns 10 if digit is not valid
uint8_t charToInt(char c) {
  if (c < '0' || '9' < c) {
    fprintf(stderr, "Char c is not a valid digit");
    return 10;
  }
  uint8_t i = (uint8_t) c - (uint8_t) '0';
  return i;
}

// Moves a char ptr to the last character in the String
// If already at null char, do nothing
void movToLastChar(char *str) {
  if (str == '\0') {
    return;
  }
  while (str != '\0') {
    str++;
  }
  str--;
}
