#ifndef TMINUS_H
#define TMINUS_H

#include <SDL/SDL_image.h>
#include "Structs.h"
#include "CircleObj.h"
#include "UtilObj.h"
#include "collisions.h"

SDL_Surface *getConsoleScreen(void);
SDL_Surface *loadImage(char *path);
void initSDL(void);
void processKeyboardInput(SDL_Event *eventPtr, bool *running);

void updateObjs(void);
void handleCollisions(void);
void drawSprites(void);

#endif
