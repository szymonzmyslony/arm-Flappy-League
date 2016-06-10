#include "UtilObj.h"

void setSprite(GameObject *gameObj, SDL_Surface *newSprite) {
  gameObj->sprite = newSprite;
}

void playWhistleSound(void) {
  Mix_PlayChannel(-1, sound_whistle, 0);
}

/** Pre: The GameObject must have v1 set to a position
*  Post: The GameObject will have its sprite centred on that position
*/
void drawCentredObj(GameObject *posObj) {
  SDL_Rect pos = { .x = posObj->v1.vec.x - (posObj->sprite->w / 2),
                   .y = posObj->v1.vec.y - (posObj->sprite->h / 2),
                   .w = 0,
                   .h = 0 };
  SDL_BlitSurface(posObj->sprite, NULL, screen, &pos);
}
