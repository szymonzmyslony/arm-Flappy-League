/*
Header File for assemble operations
*/
#ifndef ASS_OPERATIONS_H
#define ASS_OPERATIONS_H

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "helperFunctions.h"
#include "ass_symbols.h"
#include "assemble.h"

// Data Processing
uint32_t encodeDPmov(char **opFields);
uint32_t encodeDPand(char **opFields);
uint32_t encodeDPeor(char **opFields);
uint32_t encodeDPsub(char **opFields);
uint32_t encodeDPrsb(char **opFields);
uint32_t encodeDPadd(char **opFields);
uint32_t encodeDPorr(char **opFields);
uint32_t encodeDPtst(char **opFields);
uint32_t encodeDPteq(char **opFields);
uint32_t encodeDPcmp(char **opFields);
uint32_t encodeOperand2(char **opFields, uint8_t index);
uint32_t encodeDPandeq(char **opFields);
uint32_t encodeMul (char **opfields);
uint32_t encodeMla (char **opfields) ;
uint32_t encodeBeq (char **opfields);
uint32_t encodeBne (char **opfields);
uint32_t encodeBge (char **opfields);
uint32_t encodeBlt (char **opfields);
uint32_t encodeBgt (char **opfields);
uint32_t encodeBle (char **opfields);
uint32_t encodeBal (char **opfields);
uint32_t encodeSDTldr (char **opFields);
uint32_t encodeSDTstr (char **opFields);
long int expToL(char *expression, char *ptr);
uint8_t getShiftCode(char *str);
uint32_t encodelsl(char **opfields);
#endif
