#ifndef UTILOBJ_H
#define UTILOBJ_H

#include <string.h>
#include "Common.h"
#include "collisions.h"

void clearAllObjs(void);
void clearObj(GameObject *gameObj);
void clearVar(Var *var);

SDL_Surface *getRandomBirdSprite(void);
void setSprite(GameObject *gameObj, SDL_Surface *sprite);
void playWhistleSound(void);
void drawCentredObj(GameObject *collObj);
void setCollFunc(GameObject *collObj, void (*gFunc)(GameObject*));
void collBall(GameObject *collObj);
void collDoNothing(GameObject *collObj);

#endif
