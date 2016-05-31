#include "helperFunctions.h"
#include "ass_operations.h"

#define ENC_BRANCHING(name, condition)             \
uint32_t name(char **opFields){ 					\			   
	uint32_t binInstruction = 0xA000000;           \
	                                              
	binInstruction |= condition << 28;             \
	
	
   uint32_t current =  getMemAddr(); \
   int opFieldsIndex = 0; \
   char *operand = opFields[opFieldsIndex]; \

   //get Label adress
   uint32_t labelAdress = getValFromStruct(&symbolsTable, operand); 	 \
   //account for pipeline 
   current=current+8;  \
   int offset = labelAdress-current;	  \
	
   offset=offset>>2;  \
   
   binInstruction |= offset; 
		
	return binInstruction;                         \
}
ENC_BRANCHING(encodeBeq, COND_eq );
ENC_BRANCHING(encodeBne, COND_ne );
ENC_BRANCHING(encodeBge, COND_ge );
ENC_BRANCHING(encodeBlt, COND_lt);
ENC_BRANCHING(encodeBgt, COND_gt);
ENC_BRANCHING(encodeBle, COND_le);
ENC_BRANCHING(encodeBal,  COND_al);

     






uint32_t encodeMul (char **opfields) {
  // Cond is 1110, A is 0 and S is 0;
  uint32_t binInstruction = 0xC0000090;
  int opFieldsIndex = 0;

  char *operand = opFields[opFieldsIndex];
  
  //Set Rd
  
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex << 12;
  binInstruction |= regIndex32;
  opFieldsIndex++;
  
  
  //Set Rm
  
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex;
  binInstruction |= regIndex32;
  opFieldsIndex++;
  
  //set Rs
 
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex<<4; 
  binInstruction |= regIndex32;
  
}
  uint32_t encodeMla (char **opfields) {
  // Cond is 1110, A is 1 and S is 0;
  uint32_t binInstruction = 0xC0200090;
  int opFieldsIndex = 0;


  char *operand = opFields[opFieldsIndex];
  
  //Set Rd

  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex << 12;
  binInstruction |= regIndex32;
  opFieldsIndex++;
  
  
  //Set Rm
 
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex;
  binInstruction |= regIndex32;
  opFieldsIndex++;
  
  //set Rs
 
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex<<4; 
  binInstruction |= regIndex32;
   opFieldsIndex++;
   
   //set Rn
 
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex<<8; 
  binInstruction |= regIndex32;
   
   
  
}




  