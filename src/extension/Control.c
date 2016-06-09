#include "Control.h"

void moveLeft(GameObject *circObj) {
  circObj->v2.vec.x = -sideVelocity;
  circObj->v2.vec.y = -upVelocity;
}

void moveRight(GameObject *circObj) {
  circObj->v2.vec.x = +sideVelocity;
  circObj->v2.vec.y = -upVelocity;
}
