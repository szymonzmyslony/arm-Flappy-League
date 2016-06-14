#include "SquareObj.h"

#define sx       squareObj->v1.vec.x
#define sy       squareObj->v1.vec.y
#define sw       squareObj->v2.vec.x
#define sh       squareObj->v2.vec.y

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

/** An object which has a square collider. A sprite attached will be drawn
* upon the top-left corner of the squareObj. The sprite should have a width
* <= w, and a height <= h
*/
void initSquareObj(GameObject *squareObj, float x, float y, float w,
  float h, bool isSolid) {
  clearObj(squareObj);
  squareObj->objType = isSolid? COL_BOX : COL_NET;

  sx = x;
  sy = y;
  sw = w;
  sh = h;

  squareObj->update = NULL;
  squareObj->draw = &drawSquareObj;

  squareObj->v6.gFunc = &collDoNothing;
}
