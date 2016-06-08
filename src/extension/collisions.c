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

  float new_v1_x = (m1 * v1->x + m2 * v2->x + m2 * e * v2->x - m2 * e * v1->x)
          / (m1 + m2);
  float new_v1_y = (m1 * v1->y + m2 * v2->y + m2 * e * v2->y - m2 * e * v1->y)
          / (m1 + m2);
  float new_v2_x = e * v1->x + e * v2->x + new_v1_x;
  float new_v2_y = e * v1->y + e * v2->y + new_v1_y;

  v1->x = new_v1_x;
  v1->y = new_v1_y;
  v2->x = new_v2_x;
  v2->y = new_v2_y;
}
