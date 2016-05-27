#include "ass_tokeniser.h"

// Breaks a line into its label, opcode and operand fields.
// Modifies the parameters *label, *opcode and **opFields.
void tokenise (char *line, char *label, char *opcode, char **opFields) {
  char *saveptr; //TODO init
  
  label = strtok_r(line, ":", &saveptr);
  
  opcode = strtok_r(NULL, " ", &saveptr);
  
  char *opField = //TODO ALL OF THIS
  opField = strtok_r(NULL, ",", &saveptr);
  
  while(opField != NULL) {
    opField = strtok_r(NULL, ",", &saveptr);
  }
}
