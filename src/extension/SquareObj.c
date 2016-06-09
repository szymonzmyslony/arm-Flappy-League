#include "SquareObj.h"

#define sx       squareObj->v1.vec.x
#define sy       squareObj->v1.vec.y
#define sw       squareObj->v2.vec.x
#define sh       squareObj->v2.vec.y
#define collFunc squareObj->v3.func

void drawSquareObj(GameObject *squareObj) {
  if(squareObj->sprite != NULL) {
    SDL_Rect pos = { .x = sx,
                     .y = sy,
                     .w =  0,
                     .h =  0 };
    SDL_BlitSurface(squareObj->sprite, NULL, screen, &pos);
  } else {
    debugDrawSquare(sx, sy, sw, sh);
  }
}

void setCollFunc(GameObject *squareObj, void (*func)(GameObject*)) {
  squareObj->v3.gFunc = func;
}

/** An object which has a square collider. A sprite attached will be drawn
* upon the top-left corner of the squareObj.
*/
GameObject *initSquareObj(float x, float y, float w, float h, bool isSolid) {
  GameObject *squareObj = (GameObject*)calloc(1, sizeof(GameObject));
  if(squareObj == NULL) {
    printf("Error allocating gameObj memory.");
  }

  squareObj->colliderType = isSolid? COL_BOX : COL_NET;

  sx = x;
  sy = y;
  sw = w;
  sh = h;

  squareObj->draw = &drawSquareObj;

  return squareObj;
}
