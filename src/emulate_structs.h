#ifndef EMULATE_STRUCTS_H
#define EMULATE_STRUCTS_H

typedef struct processor processor;

// Structs
struct processor {
  uint32_t *registers;
  uint8_t *memory;
  int counter; // counter to determine whether to decode and execute
};

typedef struct arguments arguments;

struct arguments {
  uint8_t dRegIndex;
  uint8_t sRegIndex;
  uint8_t nRegIndex;
  uint8_t mRegIndex;
  uint8_t opCode;
  uint32_t operand2;
  uint8_t cond;
  uint32_t offset;
  void (*executePointer)(arguments *args, processor *arm);
  bool aFlag;
  bool sFlag;
  bool lFlag;
  bool iFlag;
  bool pFlag;
  bool uFlag;
};

#endif
