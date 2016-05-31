#include "helperFunctions.h"
#include "ass_operations.h"

uint32_t encodeMul (char **opfields) {
  // Cond is 1110, A is 0 and S is 0;
  uint32_t binInstruction = 0xC0000090;
  int opFieldsIndex = 0;

  char *operand = opFields[opFieldsIndex];
  
  //Set Rd
  trim(operand);
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex << 12;
  binInstruction |= regIndex32;
  opFieldsIndex++;
  
  
  //Set Rm
  trim(operand);
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex;
  binInstruction |= regIndex32;
  opFieldsIndex++;
  
  //set Rs
  trim(operand);
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
  trim(operand);
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex << 12;
  binInstruction |= regIndex32;
  opFieldsIndex++;
  
  
  //Set Rm
  trim(operand);
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex;
  binInstruction |= regIndex32;
  opFieldsIndex++;
  
  //set Rs
  trim(operand);
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex<<4; 
  binInstruction |= regIndex32;
   opFieldsIndex++;
   
   //set Rn
  trim(operand);
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex<<8; 
  binInstruction |= regIndex32;
   
   
  
}

 uint32_t encodeBeq (char **opfields) {
	  // Cond is 0000
  uint32_t binInstruction = 0xa000000;
  uint32_t current =  getMemAddr();

  int opFieldsIndex = 0;
  char *operand = opFields[opFieldsIndex];
  trim(operand);
  //get Label adress
  uint32_t labelAdress = getValFromStruct(&symbolsTable, operand); 	 
 
  //account for pipeline
  current=current+8;  
  int offset = labelAdress-current;	  
  offset=offset>>2;  
  binInstruction |= offset;
  
  
  
  
  
  
  
  
}

uint32_t encodeBne (char **opfields){
	  // Cond is 0001
	uint32_t binInstruction = 0x1a000000;
	
	
	
	
}




uint32_t encodeBge (char **opfields){
	  // Cond is 1010
	uint32_t binInstruction = 0xAA000000;
	
	
	
}

uint32_t encodeBlt (char **opfields){
	  // Cond is 1011
	uint32_t binInstruction = 0xBA000000;
	
	
	
}


uint32_t encodeBgt (char **opfields){
	  // Cond is 1100
	uint32_t binInstruction = 0xCA000000;
	
	
	
}

uint32_t encodeBle (char **opfields){
	  // Cond is 1101
	uint32_t binInstruction = 0xDA000000;
	
	
	
}

uint32_t encodeBal (char **opfields){
	  // Cond is 1110
	
	uint32_t binInstruction = 0xEA000000;
	
	
	
	
	
	
	
}




  