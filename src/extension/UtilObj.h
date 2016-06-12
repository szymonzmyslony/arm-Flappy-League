#ifndef UTILOBJ_H
#define UTILOBJ_H

#include <string.h>
#include "Common.h"
#include "collisions.h"

void clearObj(GameObject *gameObj);
void setSprite(GameObject *gameObj, SDL_Surface *sprite);
void playWhistleSound(void);
void drawCentredObj(GameObject *collObj);
void setCollFunc(GameObject *collObj, void (*gFunc)(GameObject*));
void collPlayKickSound(GameObject *collObj);
void collDoNothing(GameObject *collObj);

#endif
