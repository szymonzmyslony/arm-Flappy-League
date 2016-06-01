/*
Header File for assemble
*/
#ifndef ASSEMBLE_H
#define ASSEMBLE_H
#include <stdlib.h>
#include <ctype.h>
#include "helperFunctions.h"
#include "ass_operations.h"
#include "ass_symbols.h"
#include "ass_tokeniser.h"
#include "fileIO.h"

#define MAX_LINE_LENGTH     511
//andeq is largest opcode
#define MAX_OPCODE_LENGTH     6
#define MAX_OPFIELD_SIZE      4 //TODO check this
#define NUMBER_OF_OPCODES    23

uint32_t getMemAddr(void);
char *getFileName(void);
list *getLabelTable(void);

#endif
