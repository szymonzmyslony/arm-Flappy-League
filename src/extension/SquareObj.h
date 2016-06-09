#ifndef NETOBJ_H
#define NETOBJ_H
#include <stdbool.h>
#include "Structs.h"
#include "UtilObj.h"
#include "Debug.h"

GameObject *initSquareObj(float x, float y, float w, float h, bool isSolid);
void drawSquareObj(GameObject *squareObj);
void setAsGoal(GameObject *scoreObj);
void setCollFunc(GameObject *squareObj, void (*func)(GameObject*));

#endif
