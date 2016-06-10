#ifndef UTILOBJ_H
#define UTILOBJ_H

#include "Common.h"
#include "collisions.h"

void setSprite(GameObject *gameObj, SDL_Surface *sprite);
void playWhistleSound(void);
void drawCentredObj(GameObject *collObj);

#endif
