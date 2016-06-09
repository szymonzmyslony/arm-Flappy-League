#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "Structs.h"

enum colliderTypes {
  COL_CIRCLE = 1,
  COL_LINE   = 2,
  COL_NET    = 3,
  COL_BOX    = 4
};

void resolveCollision(Vector *v1, Vector *v2, int m1, int m2, float e);
void resolveCollisionAdvanced(Vector *v1, Vector *v2, int m1, int m2,
                              Vector *c1, Vector *c2, float e);
bool circlesCollided(GameObject *c1, GameObject *c2);
bool circleBoxCollided(GameObject *c, GameObject *b);
bool circleNetCollided(GameObject *c, GameObject *b);

float vectorLength(Vector *v1);
float dotProduct(Vector *v1, Vector *v2);
Vector scalarMultiply(float scalar, Vector *v1);
Vector getPerpVect(Vector *v1, Vector *parallel_vect);
void resolveVectorParts(Vector *v1, Vector *para, Vector *perp);

#endif
