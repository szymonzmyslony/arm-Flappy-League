#ifndef EMULATE_H
#define EMULATE_H

uint32_t setBit(uint32_t word, bool set, uint8_t position);
uint32_t fetch(struct processor arm);
void mul(struct arguments decodedArgs, struct processor arm);
void setFlagsMul(uint32_t value, struct processor arm);


#endif
