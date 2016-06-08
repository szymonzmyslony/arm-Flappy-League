#include "CircleObj.h"

#define cx     circObj->v1.vec.x
#define cy     circObj->v1.vec.y
#define cvx    circObj->v2.vec.x
#define cvy    circObj->v2.vec.y
#define radius     circObj->v3.f

/** An object which has a circle collider. A sprite attached will be centred
* upon the centre of the circle
*/
void drawCircleObj(GameObject *circObj) {
  if(circObj->sprite != NULL) {
    drawCentredObj(circObj);
  } else {
    debugDrawCircle(cx, cy, radius);
  }
}

void updateCircleObj(GameObject *circObj) {

}

GameObject *initCircleObj(float r, float x, float y, float vx, float vy) {
  GameObject *circObj = (GameObject*)calloc(1, sizeof(GameObject));

  cx = x;
  cy = y;

  cvx = vx;
  cvy = vy;

  radius = r;

  circObj->draw = &drawCircleObj;
  circObj->update = &updateCircleObj;
}

/** Pre: The GameObject must have v1 set to a position
*  Post: The GameObject will have its sprite centred on that position
*/
void drawCentredObj(GameObject *gameObj) {
  SDL_Rect pos = { .x = gameObj->v1.vec.x - (gameObj->sprite->w / 2),
                   .y = gameObj->v1.vec.y - (gameObj->sprite->h / 2),
                   .w = 0,
                   .h = 0 };
  SDL_BlitSurface(gameObj->sprite, NULL, screen, &pos);
}
