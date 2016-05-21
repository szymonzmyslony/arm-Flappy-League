#ifndef EMULATE_H
#define EMULATE_H

// Structs
struct processor;
struct arguments;

// Helper Functions
uint32_t setBit(uint32_t word, bool set, uint8_t position);

// Decode Functions
void decodeDP(int dInstruction, struct arguments decodedArgs);

void opDPand(struct arguments decodedArgs, struct processor arm);
void opDPandWithFlags(struct arguments decodedArgs, struct processor arm);

void opDPeor(struct arguments decodedArgs, struct processor arm);
void opDPeorWithFlags(struct arguments decodedArgs, struct processor arm);

void opDPorr(struct arguments decodedArgs, struct processor arm);
void opDPorrWithFlags(struct arguments decodedArgs, struct processor arm);

void opDPmov(struct arguments decodedArgs, struct processor arm);

void opDPsub(struct arguments decodedArgs, struct processor arm);
void opDPsubWithFlags(struct arguments decodedArgs, struct processor arm);

void opDPrsb(struct arguments decodedArgs, struct processor arm);
void opDPrsbWithFlags(struct arguments decodedArgs, struct processor arm);

void opDPadd(struct arguments decodedArgs, struct processor arm);
void opDPaddWithFlags(struct arguments decodedArgs, struct processor arm);

void opDPtst(struct arguments decodedArgs, struct processor arm);
void opDPteq(struct arguments decodedArgs, struct processor arm);
void opDPcmp(struct arguments decodedArgs, struct processor arm);

void opDPNothing(struct arguments decodedArgs, struct processor arm);

void setFlagsDP(uint32_t value, struct processor arm);

#endif
