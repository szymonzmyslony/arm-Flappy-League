#include "ass_tokeniser.h"

// Breaks a line into its label, opcode and operand fields.
// Modifies the arguments *label, *opcode and **opFields.
void tokenise (char *line, char *label, char *opcode, char **opFields) {  
  // Set the label, if it exists
  strcpy(label, strtok(line, ":"));
  
  // Set the opcode
  strcpy(opcode, strtok(NULL, " "));
  
  // Set the opFields
  int index = 0;
  char opField[MAX_OPFIELD_SIZE];
  do {
    strcpy(opField, strtok(NULL, ","));
    strcpy(opFields[index], opField);
    index++;
  } while(opField != NULL);
}