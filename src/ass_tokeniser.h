#ifndef ASS_TOKENISER_H
#define ASS_TOKENISER_H
#include <string.h>
#include "helperFunctions.h"
#define MAX_OPFIELD_LENGTH 20;

tokenise (char *line, char *label, char *opCode, char **opFields);

#endif
