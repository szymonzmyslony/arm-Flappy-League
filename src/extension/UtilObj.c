#include "UtilObj.h"

#define collFunc collObj->v6.gFunc

void clearObj(GameObject *gameObj) {
  memset(gameObj, 0, sizeof(GameObject));
}

void setSprite(GameObject *gameObj, SDL_Surface *newSprite) {
  gameObj->sprite = newSprite;
}

void setCollFunc(GameObject *collObj, void (*gFunc)(GameObject*)) {
  collFunc = gFunc;
}

void playWhistleSound(void) {
  Mix_PlayChannel(-1, sound_whistle, 0);
}

void playCollKickSound(GameObject *collObj) {
  switch((int)(gObjs[BALL]->v1.vec.x) % 4) {
    case 0: Mix_PlayChannel(-1, sound_kick1, 0); break;
    case 1: Mix_PlayChannel(-1, sound_kick2, 0); break;
    case 2: Mix_PlayChannel(-1, sound_kick3, 0); break;
    case 3: Mix_PlayChannel(-1, sound_kick4, 0); break;
  }
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
