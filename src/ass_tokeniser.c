#include "ass_tokeniser.h"

// Breaks a line into its label, opCode and operand fields.
// Modifies all arguments.
void tokenise (char *line, char *opCode, char **opFields) {
  // Set the opCode
  strcpy(opCode, trim(strtok(NULL, " ")));

  // Set the opFields
  char opField[MAX_OPFIELD_LENGTH];
  strcpy(opField, strtok(NULL, ","));
  for(int i = 0; opField != NULL; i++) {
	  strcpy(opFields[i], trim(opField));
	  strcpy(opField, strtok(NULL, ","));
  }
}
