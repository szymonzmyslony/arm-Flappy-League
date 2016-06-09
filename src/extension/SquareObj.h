#ifndef NETOBJ_H
#define NETOBJ_H
#include "UtilObj.h"

GameObject *initSquareObj(float x, float y, float w, float h, bool isSolid);
void drawSquareObj(GameObject *squareObj);
void setAsGoal(GameObject *scoreObj);

#endif
