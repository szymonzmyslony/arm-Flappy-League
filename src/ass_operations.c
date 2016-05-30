#include "helperFunctions.h"
#include "ass_operations.h"

uint32_t encodeMul (char **opfields) {
  // Cond is 1110, A is 0 and S is 0;
  uint32_t binInstruction = 0xC0000090;
  int opFieldsIndex = 0;

  char *operand = opFields[opFieldsIndex];
  
  //Set Rd
  removeLeadingSpace(operand);
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex << 12;
  binInstruction |= regIndex32;
  opFieldsIndex++;
  
  
  //Set Rm
  removeLeadingSpace(operand);
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex;
  binInstruction |= regIndex32;
  opFieldsIndex++;
  
  //set Rs
  removeLeadingSpace(operand);
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
  removeLeadingSpace(operand);
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex << 12;
  binInstruction |= regIndex32;
  opFieldsIndex++;
  
  
  //Set Rm
  removeLeadingSpace(operand);
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex;
  binInstruction |= regIndex32;
  opFieldsIndex++;
  
  //set Rs
  removeLeadingSpace(operand);
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex<<4; 
  binInstruction |= regIndex32;
   opFieldsIndex++;
   
   //set Rn
   removeLeadingSpace(operand);
  uint8_t regIndex = getRegIndex(operand);
  uint32_t regIndex32 = regIndex<<8; 
  binInstruction |= regIndex32;
   
   
  
}

 uint32_t encodeBeq (char **opfields) {
  // Cond is 0000
  uint32_t binInstruction = 0xA000000;
  
  int opFieldsIndex = 0;
  char *operand = opFields[opFieldsIndex];
  

  removeLeadingSpace(operand);
  //get label adress
  
  
}


  