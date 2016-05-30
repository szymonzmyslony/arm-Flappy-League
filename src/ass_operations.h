#ifndef ASS_OPERATIONS_H
#define ASS_OPERATIONS_H

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

#endif
