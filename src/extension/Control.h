#ifndef CONTROL_H
#define CONTROL_H
#include "Structs.h"
#include "Tminus.h"
#define upVelocity   10.0
#define sideVelocity  5.0

void moveLeft(GameObject *circObj);
void moveRight(GameObject *circObj);

void check_left_first(void);
void check_right_first(void);
void check_left_second(void);
void check_right_second(void);

#endif
