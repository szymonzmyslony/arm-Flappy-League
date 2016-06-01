#include "ass_tokeniser.h"

// Breaks a line into its label, opCode and operand fields.
// Modifies all arguments.
void tokenise (char *line, char *opcode, char **opFields) {
  char *token = strtok(line, " ");
  trim(token);
  // Set the opcode
  strcpy(opcode, token);

  // Set the opFields
  token = strtok(NULL, ",");
  for(int i = 0; token != NULL; i++) {
  	  trim(token);
	  strcpy(opFields[i], token);
	  token = strtok(NULL, ",");
  }
}
