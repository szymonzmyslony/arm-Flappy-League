#include "collisions.h"

void resolveCollision(Vector *v1, Vector *v2, int m1, int m2, float e) {
  //          m1u1 + m2u2 = m1v1 + m2v2

  //                    e = (sep speed)/(approach speed)
  //                    e = (v2 - v1)/(u1 - u2)
  //            eu1 - eu2 = v2 - v1
  //                   v2 = eu1 - eu2 + v1
  //                   v2 = (m1u1 + m2u2 - m1v1) / m2
  //       eu1 - eu2 + v1 = (m1u1 + m2u2 - m1v1) / m2
  // m2eu1 - m2eu2 + m2v1 = m1u1 + m2u2 - m1v1
  //          m1v1 + m2v1 = m1u1 + m2u2 + m2eu2 - m2eu1
  //          v1(m1 + m2) = m1u1 + m2u2 + m2eu2 - m2eu1
  //                   v1 = (m1u1 + m2u2 + m2eu2 - m2eu1) / (m1 + m2)

  float new_v1_x = (m1 * v1->x + m2 * v2->x + m2 * e * v2->x - m2 * e * v1->x) / (m1 + m2);
  float new_v2_x = e * v1->x - e * v2->x + new_v1_x;

  float new_v1_y = (m1 * v1->y + m2 * v2->y + m2 * e * v2->y - m2 * e * v1->y) / (m1 + m2);
  float new_v2_y = e * v1->y - e * v2->y + new_v1_y;

  v1->x = new_v1_x;
  v2->x = new_v2_x;

  v1->y = new_v1_y;
  v2->y = new_v2_y;
}

bool circlesCollided(GameObject *c1, GameObject *c2) {
  float dx = c1->v1.vec.x - c2->v1.vec.x;
  float dy = c1->v1.vec.y - c2->v1.vec.y;

  float dr = c1->v3.f + c2->v3.f;

  return (dx * dx) + (dy * dy) < dr * dr;
}

bool circleBoxCollided(GameObject *c, GameObject *b) {

}

bool circleNetCollided(GameObject *c, GameObject *b) {
  float cLeft   = c->v1.vec.x - c->v3.f;
  float cRight  = c->v1.vec.x + c->v3.f;
  float cTop    = c->v1.vec.y - c->v3.f;
  float cBottom = c->v1.vec.y + c->v3.f;

  float bLeft   = b->v1.vec.x;
  float bRight  = b->v1.vec.x + b->v2.vec.x;
  float bTop    = b->v1.vec.y;
  float bBottom = b->v1.vec.y + b->v2.vec.y;

  return (cLeft >= bLeft) && (cRight <= bRight)
         && (cTop >= bTop) && (cBottom <= bBottom);
}
