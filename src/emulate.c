#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include "emulate.h"

#define NUMBER_OF_REGISTERS   17
#define BYTES_IN_MEMORY       65536
#define INSTRUCTION_BYTES      4
// index of Program Counter in registers array
#define PC                    15
// index of CPSR in registers array
#define CPSR                  16
// bit flag indexes
#define Vbit                  28
#define Cbit                  29
#define Zbit                  30
#define Nbit                  31

// masks for multiply
#define Abit                  21
#define Sbit                  20
                  
// masks
#define MASK3_0               0x0000000F
#define MASK11_8              0x00000F00
#define MASK15_12             0x0000F000
#define MASK19_16             0x000F0000
#define MASK24_21             0x01E00000
#define MASK0_23			  0x00ffffff


//offset
#define numberofelements 24

struct processor {
  uint32_t registers[NUMBER_OF_REGISTERS];	 
  uint8_t memory[BYTES_IN_MEMORY];
  bool end;
  int counter; // counter to determine whether to decode and execute
};

struct arguments {
  uint8_t dRegIndex;
  uint8_t sRegIndex;
  uint8_t nRegIndex;
  uint8_t mRegIndex;
  uint16_t operand2;
  uint8_t cond;
  uint32_t offset;
  bool aFlag;
  bool sFlag;
  bool iFlag;
 
  void (*executePointer)(struct arguments args, struct processor arm); 
};



 void loadFile(char name[],struc processor *pointer){	  
	FILE *myFile;
	myFile = fopen(name, "r");
	fread(*(pointer->memory), 4, BYTES_IN_MEMORY, myFile);
	//library funtion that reads binary words
	
	
  }
  
  void print(uint32_t arr[], uint32_t length){
// funtion for printing array of given lenght
	int i;
	for(i=0; i<length; ++i){
		printf("Register no %d holds value %d.\n", i+1, arr[i]);
		
	}




}
  
  
  
  //decode branching 
  void decodeDP(int dInstruction, struct arguments *decodedArgs) {
  // - decode operation
  decodedArgs->executePointer = &execBranching;
  //decode offset (still unsgined)
  uint32_t mask = MASK0_23;
  decodedArgs->offset = (dInstruction & mask)
  
  
  
  
  }
  
  
  //execute branching
  void execBranching(struct arguments decodedArgs, struct processor *	arm) {
		
   bool negative = ((decodedArgs->offset)>>(numberofelements-1));
   uint32_t trueoffset = ~(decodedArgs->offset);
   trueoffset++; 
   if (negative){
	arm->registers[PC] = (arm->registers[PC]-trueoffset);
   }   
   else{
	   arm->registers[PC] = (arm->registers[PC]+trueoffset);
	   
	   
   }
   
   arm->counter=0;
   
   
   
}



uint32_t fetch(struct processor *arm) {
  return   (arm->memory[arm->registers[PC] + 3] << 24) 
          + (arm->memory[arm->registers[PC] + 2] << 16)
          + (arm->memory[arm->registers[PC] + 1] <<  8)
+  arm->memory[arm->registers[PC]];}





int main(int argc, char **argv) {
  
  struct processor arm;
  struct arguments decodedArgs;

  // load program from args and initialise processor and read from file
  loadFile(argv[0], &arm);
  
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
    dInstruction = fetch(&arm);
    
    // increment program counter 
    arm.registers[PC] += INSTRUCTION_BYTES;

    // Increment counter but avoid overflow of counter
    if (arm.counter < 3){
      arm.counter++;
    }
  }
  
  // print register states
	print(arm.registers, NUMBER_OF_REGISTERS);
  
  

  return EXIT_SUCCESS;
}



// Returns the instruction in the byte order as shown in the specification
// and increments the program counter









// ====================== Helper Functions ====================================

// Returns a given word except with a single bit set in the given position
uint32_t setBit(uint32_t word, bool set, uint8_t position) {
  //Clears the bit at the given position and then sets it
  return (word & ~(1 << position)) | (set << position);
}
