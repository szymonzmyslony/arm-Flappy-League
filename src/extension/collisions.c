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

  float new_v1_x = (m1 * v1->x + m2 * v2->x + m2 * e * v2->x - m2 * e * v1->x) /
          (m1 + m2);
  float new_v2_x = e * v1->x - e * v2->x + new_v1_x;

  float new_v1_y = (m1 * v1->y + m2 * v2->y + m2 * e * v2->y - m2 * e * v1->y) /
          (m1 + m2);
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

//Unimplemented
bool circleBoxCollided(GameObject *c, GameObject *b) {
  return false;
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

/* Resolves a collision between two circles where the circles do not move on
 * the line of collision
 * v1, and v2 are the movement vector of the objects and c1, c2 are their
 * centres */
void resolveCollisionAdvanced(Vector *v1, Vector *v2, int m1, int m2,
                              Vector *c1, Vector *c2, float e) {
  Vector line_of_collision;
  line_of_collision.x = c1->x - c2->x;
  line_of_collision.y = c1->y - c2->y;
  float distance = vectorLength(&line_of_collision);

  // so as not to divide by zero
  if (distance == 0.0) {
    distance = 1.0;
    line_of_collision.x = 1;
    line_of_collision.y = 0;
  }

  // make line_of_collision a unit vector
  line_of_collision.x = line_of_collision.x / distance;
  line_of_collision.y = line_of_collision.y / distance;

  // get parallel component of v1
  float v1_parallel_scalar = dotProduct(v1, &line_of_collision);

  Vector v1_parallel_vect = scalarMultiply(v1_parallel_scalar,
                                           &line_of_collision);

  // get parallel component of v2
  float v2_parallel_scalar = dotProduct(v2, &line_of_collision);

  Vector v2_parallel_vect = scalarMultiply(v2_parallel_scalar,
                                           &line_of_collision);

  // get the perpendicular component of v1
  Vector v1_perp = getPerpVect(v1, &v1_parallel_vect);

  // get the perpendicular component of v2
  Vector v2_perp = getPerpVect(v2, &v2_parallel_vect);

  // resolve and set the parallel parts
  resolveCollision(&v1_parallel_vect, &v2_parallel_vect, m1, m2, e);

  // find the resultant vector of two perpendicular vectors
  resolveVectorParts(v1, &v1_perp, &v1_parallel_vect);
  resolveVectorParts(v2, &v2_perp, &v2_parallel_vect);
}

/* Returns the length of a given vector */
float vectorLength(Vector *v1) {
  return sqrtf((v1->x * v1->x) + (v1->y * v1->y));
}

/* Returns the vector that is the dot product of two given vectors */
float dotProduct(Vector *v1, Vector *v2) {
  return (v1->x * v2->x) + (v1->y * v2->y);
}

/* Multiplies a vector by a scalar and returns it */
Vector scalarMultiply(float scalar, Vector *v1) {
  Vector ret_vect;
  ret_vect.x =  v1->x * scalar;
  ret_vect.y =  v1->y * scalar;
  return ret_vect;
}

/* Returns the part of a vector that is perpendicular to a given vector */
Vector getPerpVect(Vector *v1, Vector *parallel_vect) {
  Vector perp_vect;
  perp_vect.x = v1->x - parallel_vect->x;
  perp_vect.y = v1->y - parallel_vect->y;
  return perp_vect;
}

/* Sets the vecotr v1 to the resultant of the two perpendicular vectors */
void resolveVectorParts(Vector *v1, Vector *para, Vector* perp) {
  v1->x = para->x + perp->x;
  v1->y = para->y + perp->y;
}
