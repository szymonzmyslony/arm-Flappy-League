#include "UtilObj.h"

#define collFunc collObj->v6.gFunc

void clearAllObjs(void) {
  for(int i = 0; i < MAX_OBJECTS; i++) {
    clearObj(gObjs[i]);
  }
}

void clearObj(GameObject *gameObj) {
  memset(gameObj, 0, sizeof(GameObject));
}

void clearVar(Var *var) {
  memset(var, 0, sizeof(Var));
}

void setSprite(GameObject *gameObj, SDL_Surface *newSprite) {
  gameObj->sprite = newSprite;
}

SDL_Surface *getRandomBirdSprite(void) {
  switch(rand() % 4) {
    case 0: return surf_bird1;
    case 1: return surf_bird2;
    case 2: return surf_bird3;
    case 3: return surf_bird4;
    default: return surf_datboi;
  }
}

void setCollFunc(GameObject *collObj, void (*gFunc)(GameObject*)) {
  collFunc = gFunc;
}

void playWhistleSound(void) {
  Mix_PlayChannel(-1, sound_whistle, 0);
}

/* Collision function for the ball. Plays a kick sound and tracks stats.
*/
void collBall(GameObject *collObj) {
  switch((int)(gObjs[BALL]->v1.vec.x) % 4) {
    case 0: Mix_PlayChannel(-1, sound_kick1, 0); break;
    case 1: Mix_PlayChannel(-1, sound_kick2, 0); break;
    case 2: Mix_PlayChannel(-1, sound_kick3, 0); break;
    case 3: Mix_PlayChannel(-1, sound_kick4, 0); break;
  }

  // Collided with a circObj
  if(collObj == gObjs[PLAYER1]) {
    gObjs[STATS]->v1.vec.x++;
  } else if(collObj == gObjs[PLAYER2]) {
    gObjs[STATS]->v1.vec.y++;
  }
}

void collDoNothing(GameObject *collObj) {}

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
