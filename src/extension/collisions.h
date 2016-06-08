#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <stdlib.h>
#include <stdbool.h>
#include "Structs.h"

enum colliderTypes {
  COL_CIRCLE = 1,
  COL_LINE   = 2,
  COL_NET    = 3,
};

void resolveCollision(Vector *v1, Vector *v2, int m1, int m2, float e);
bool circlesCollided(GameObject *c1, GameObject *c2);

#endif
