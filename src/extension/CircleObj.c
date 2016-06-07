#include <SDL/SDL_draw.h>

/** An object which has a circle collider. A sprite attached will be centred
* upon the centre of the circle
*/
GameObject *initCircleObj(float r, float x, float y, float vx, float vy) {
  GameObject *circObj = (GameObject*)calloc(1, sizeof(GameObject));
  // First variable: position
  circObj->v1.vec.x = x;
  circObj->v1.vec.y = y;
  // Second variable: velocity
  circObj->v2.vec.x = vx;
  circObj->v2.vec.y = vy;

  // Third variable: radius
  circObj->v3.f = r;

  circObj->draw = &drawCircleObj;
  circObj->update = &updateCircleObj;
}

void drawCircleObj(GameObject *circObj) {
  if(circObj->sprite != NULL) {
    drawCentredObj(circObj);
  } else {
    Draw_Circle(screen, circObj->v1.vec.x, circObj->v1.vec.y, circObj->v3.f,
                0xaabbccdd);
  }
}

void updateCircleObj(GameObject *circObj) {
  
}

/** Pre: The GameObject must have v1 set to a position
*  Post: The GameObject will have its sprite centred on that position
*/
void drawCentredObj(GameObject *gameObj) {
  SDL_Rect pos = { .x = gameObj->v1.vec.x - (gameObj->sprite.w / 2),
                   .y = gameObj->v1.vec.y - (gameObj->sprite.h / 2),
                   .w = 0,
                   .h = 0 };
  SDL_BlitSurface(circObj->sprite, NULL, screen, &pos);
}
