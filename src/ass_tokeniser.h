/*
Header File for tokeniser used in assemble
*/

#ifndef ASS_TOKENISER_H
#define ASS_TOKENISER_H
#include <string.h>
#include "helperFunctions.h"
#define MAX_OPFIELD_LENGTH 20

void tokenise (char *line, char *opCode, char **opFields);

#endif
