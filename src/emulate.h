#ifndef EMULATE_H
#define EMULATE_H
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#define NUMBER_OF_REGISTERS   17
#define BYTES_IN_MEMORY       65536
#define INSTRUCTION_BYTES      4
// index of Program Counter in registers array
#define PC                    15
// index of CPSR in registers array
#define CPSR                  16
// bit flag indexes
#define Lbit                  20
#define Ubit                  23
#define Pbit                  24
#define Ibit                  25
#define Vbit                  28
#define Cbit                  29
#define Zbit                  30
#define Nbit                  31
#define numberofelements      24

// masks for multiply
#define Abit                  21
#define Sbit                  20
                  
// masks
#define MASK3_0               0x0000000F
#define MASK11_8              0x00000F00
#define MASK15_12             0x0000F000
#define MASK19_16             0x000F0000
#define MASK24_21             0x01E00000
#define MASK0_23              0x00ffffff
#define MASK11_7              0x00000f80
#define MASK11_0              0x00000fff
#define MASK7_0               0x000000ff
#define MASK6_5               0x00000060

// opcodes
#define OPCODE_and             0
#define OPCODE_eor             1
#define OPCODE_sub             2
#define OPCODE_rsb             3
#define OPCODE_add             4
#define OPCODE_tst             8
#define OPCODE_teq             9
#define OPCODE_cmp            10
#define OPCODE_orr            12
#define OPCODE_mov            13

// condition cases
#define COND_eq                0
#define COND_ne                1
#define COND_ge               10
#define COND_lt               11
#define COND_gt               12
#define COND_le               13
#define COND_al               14

//Output
#define REG_PADDING            4
#define DEC_PADDING           11


// Structs
struct processor;
struct arguments;

// Main flow
void execute(struct arguments *decodedArgs, struct processor *arm);
void decode(uint32_t dInstruction, struct arguments *decodedArgs);
uint32_t fetch(struct processor *arm);
void initProcessor(struct processor *arm);
void printReg(uint32_t arr[], uint32_t length);
void printMem(uint8_t arr[], uint32_t length);
void loadFile(char name[], struct processor *pointer);
void printSingleRegister(char regName[], uint32_t num);
void printPaddedNum(uint32_t num);

// Data Processing
void executeDP(struct arguments *decodedArgs, struct processor *arm);
void decodeDP(int dInstruction, struct arguments *decodedArgs);

// Branching
void decodeBranching(int dInstruction, struct arguments *decodedArgs);
void execBranching(struct arguments *decodedArgs, struct processor *arm);
uint32_t signEx24to32(uint32_t val24);

// Multiply
void decodeMul(int dInstruction, struct arguments *decodedArgs);
void mul(struct arguments *decodedArgs, struct processor *arm);
void setFlagsMul(uint32_t value, struct processor *arm);


// Single Data Transfter
void decodeSDT(uint32_t dInstruction, struct arguments *decodedArgs);
void execSDT(struct arguments *decodedArgs, struct processor *arm);
void ldrSDTpre(struct arguments *decodedArgs, struct processor *arm);
void ldrSDTpost(struct arguments *decodedArgs, struct processor *arm);
void strSDTpre(struct arguments *decodedArgs, struct processor *arm);
void strSDTpost(struct arguments *decodedArgs, struct processor *arm);

// Error functions
void checkAllocError(void* ptr, uint32_t numBytes);
void printFileNotFoundError(char fileName[]);
bool outOfBounds(uint32_t memAddress);
void printOOBError(uint32_t memAddress);

// Helper Functions which use struct
void setFlagsZN(uint32_t value, struct processor *arm);
uint32_t getLittleFromMem32(uint32_t address, struct processor *arm);
void storeBigEndy32(uint32_t value, uint32_t address, struct processor *arm);
void resolveSDTOffset(uint16_t offset, bool iFlag,
        struct arguments *decodedArgs, struct processor *arm);
void resolveOperand2(uint32_t op, bool iFlag, struct arguments *decodedArgs,
        struct processor *arm);
uint32_t shift(uint8_t shiftCode, uint32_t value, uint16_t,
        struct processor *arm, bool sFlag);
#endif
