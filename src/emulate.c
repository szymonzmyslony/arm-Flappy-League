#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>

#define NUMBER_OF_REGISTERS   17
#define BYTES_IN_MEMORY       65536
// index of Program Counter in registers array
#define PC                    15
// index of CPSR in registers array
#define CPSR                  16

struct processor {
  uint32_t registers[NUMBER_OF_REGISTERS];	 
  uint8_t memory[BYTES_IN_MEMORY];
  bool end;
  int counter; // counter to detewrmine whether to decode and execute
};

struct arguments {
  uint8_t dRegIndex;
  uint8_t sRegIndex;
  uint8_t nRegIndex;
  uint8_t mRegIndex;
  uint16_t operand2;
  uint8_t cond;
  uint32_t offset;
  void (*executePointer)(struct arguments args); 
};


int main(int argc, char **argv) {
  
  struct processor arm;
  struct arguments decodedArgs;

  // load program from args and initialise processor and read from file


  // points to appropriate execute function after decoding
  uint32_t dInstruction; // instruction to be decoded next

  while (!arm.end) {
    
    // execute instruction
    if (arm.counter >= 2) {
      execute(decodedArgs, arm);
    }

    // decode instruction
    if (arm.counter >= 1) {
      decode(dInstruction, decodedArgs);
    }

    // fetch instruction
    dInstruction = fetch(dInstruction, arm);

    // Increment counter but avoid overflow of counter
    if (counter < 3){
      arm.counter++;
    }
  }
  
  // print register states

  return EXIT_SUCCESS;
}

