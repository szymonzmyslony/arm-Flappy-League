#ifndef CIRCLE_OBJ_H
#define CIRCLE_OBJ_H

#include "Structs.h"
#include "Debug.h"
#include "collisions.h"
#include "UtilObj.h"

void *initCircleObj(GameObject *circObj, float r, float x, float y, float vx,
  float vy);
void drawCircleObj(GameObject *circObj);
void updateCircleObj(GameObject *circObj);

#endif
