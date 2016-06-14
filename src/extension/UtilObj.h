#ifndef UTILOBJ_H
#define UTILOBJ_H

#include <string.h>
#include "Common.h"
#include "collisions.h"

enum utilSizes {
  NUM_XGAP = 8,
  NUM_WIDTH = SURF_NUMBERS_TWIDTH,
  NUM_HEIGHT = SURF_NUMBERS_HEIGHT,
  NUM_3WIDTH = 3 * NUM_WIDTH + 2 * NUM_XGAP
};

void clearAllObjs(void);
void clearObj(GameObject *gameObj);
void clearVar(Var *var);

SDL_Surface *getRandomBirdSprite(void);
void setSprite(GameObject *gameObj, SDL_Surface *sprite);
void playWhistleSound(void);
void setCollFunc(GameObject *collObj, void (*gFunc)(GameObject*));
void collBall(GameObject *collObj);
void collDoNothing(GameObject *collObj);
void drawCentredObj(GameObject *collObj);
void animate(SDL_Surface *animationSource, int i, int j, int width, int height,
  int x, int y);

void drawNum(int num, int x, int y);

#endif
