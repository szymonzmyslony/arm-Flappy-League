#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H
#include <ctype.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "emulate_structs.h"
#include "ass_symbols.h"

#define NUMBER_OF_REGISTERS   17
#define BYTES_IN_MEMORY       65536
#define INSTRUCTION_BYTES      4
// index of Program Counter in registers array
#define PC                    15
// index of CPSR in registers array
#define CPSR                  16
// bit flag indexes
#define Abit                  21
#define Sbit                  20
#define Lbit                  20
#define Ubit                  23
#define Pbit                  24
#define Ibit                  25
#define Vbit                  28
#define Cbit                  29
#define Zbit                  30
#define Nbit                  31
#define numberOfElements      24

// masks
#define MASK3_0               0x0000000F
#define MASK11_8              0x00000F00
#define MASK15_12             0x0000F000
#define MASK19_16             0x000F0000
#define MASK24_21             0x01E00000
#define MASK0_23              0x00FFFFFF
#define MASK11_7              0x00000F80
#define MASK11_0              0x00000FFF
#define MASK7_0               0x000000FF
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

// Helper functions which do not use Structs
uint32_t switchEndy32(uint32_t value);
bool getBit(uint32_t word, uint8_t position);
uint32_t setBit(uint32_t word, bool set, uint8_t position);
uint32_t rotateRight32(uint32_t val, uint16_t n);
uint32_t arithShiftRight32(uint32_t val, uint16_t n);
uint8_t getRegIndex(char *str);
void movToLastChar(char *str);
uint8_t charToInt(char c);
void trim(char *string);
uint32_t signEx24to32(uint32_t val24);

// Helper functions which use structs
void setZNFlags(uint32_t value, processor *arm);
uint32_t getLittleFromMem32(uint32_t address, processor *arm);
void storeBigEndy32(uint32_t value, uint32_t address, processor *arm);
void resolveSDTOffset(uint16_t offset, bool iFlag,
        arguments *decodedArgs, processor *arm);
void resolveOperand2(uint32_t op, bool iFlag, arguments *decodedArgs,
        processor *arm);
uint32_t shift(uint8_t shiftCode, uint32_t value, uint16_t,
        processor *arm, bool sFlag);
#endif
