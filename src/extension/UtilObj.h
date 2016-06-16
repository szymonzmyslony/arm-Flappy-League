#ifndef UTILOBJ_H
#define UTILOBJ_H

#include <string.h>
#include "Common.h"
#include "collisions.h"
#include "CircleObj.h"

#define ROT_SCALE 12.0

enum utilSizes {
  NUM_XGAP = 4,
  NUM_WIDTH = SURF_NUMBERS_TWIDTH,
  NUM_HEIGHT = SURF_NUMBERS_HEIGHT,
  NUM_3WIDTH = 3 * NUM_WIDTH + 2 * NUM_XGAP
};

void clearAllObjs(void);
void clearObj(GameObject *gameObj);
void clearVar(Var *var);

SDL_Surface *getRandomBirdSprite(void);
void updatePlayer(GameObject *circObj);
void updateBall(GameObject *circObj);
void setSprite(GameObject *gameObj, SDL_Surface *sprite);
void playWhistleSound(void);
void setCollFunc(GameObject *collObj, void (*gFunc)(GameObject*));
void collBall(GameObject *collObj);
void collDoNothing(GameObject *collObj);
void collPlayer(GameObject *collObj);
void drawCentredObj(GameObject *collObj);
void drawAnimCircObj(GameObject *collObj);
void animate(SDL_Surface *animationSource, int i, int j, int width, int height,
  int x, int y);

void drawNum(int num, int x, int y);

#endif
