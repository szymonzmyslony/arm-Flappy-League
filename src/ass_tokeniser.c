#include "ass_tokeniser.h"

// Breaks a line into its label, opcode and operand fields.
// Modifies all arguments.
void tokenise (char *line, char *opcode, char **opFields) {
  // Set the opcode
  strcpy(opcode, trim(strtok(NULL, " ")));
  
  // Set the opFields
  char opField[MAX_OPFIELD_SIZE];
  strcpy(opField, strtok(NULL, ","));
  for(int i = 0; opField != NULL; i++) {
	  strcpy(opFields[index], trim(opField));
	  strcpy(opField, strtok(NULL, ","));
  }
}
