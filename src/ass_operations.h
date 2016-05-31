#ifndef ASS_OPERATIONS_H
#define ASS_OPERATIONS_H

uint32_t encodeSDTldr (char **opFields);
uint32_t encodeSDTstr (char **opFields);
long int expToL(char *expression, char *ptr);
void removeLeadingSpace(char *str);
uint8_t getShiftCode(char *str);
uint32_t encodelsl(char **opfields);

#endif
