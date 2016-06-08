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
  cx += cvx;
  cy += cvy;

  if((cx - radius < 0 && cvx < 0) || (cx + radius > screen->w && cvx > 0)) {
    cvx = -cvx;
  }

  if((cy - radius < 0 && cvy < 0)) {
    cvy = 0;
  }
  if(cy + radius > screen->h && cvy > 0) {
    cvy = -15;
  }
}

GameObject *initCircleObj(float r, float x, float y, float vx, float vy) {
  GameObject *circObj = (GameObject*)calloc(1, sizeof(GameObject));
  if(circObj == NULL) {
    printf("Error allocating gameObj memory.");
    //TODO do for all
  }

  circObj->colliderType = COL_CIRCLE;

  radius = r;

  cx = x;
  cy = y;

  cvx = vx;
  cvy = vy;

  circObj->draw = &drawCircleObj;
  circObj->update = &updateCircleObj;

  return circObj;
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
