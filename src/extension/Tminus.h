#ifndef TMINUS_H
#define TMINUS_H

#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include "Structs.h"
#include "CircleObj.h"
#include "UtilObj.h"
#include "collisions.h"
#include "Control.h"
#include "GameEdit.h"

SDL_Surface *getConsoleScreen(void);
SDL_Surface *loadImage(char *path);
Mix_Music *loadMusic(char *path);
Mix_Chunk *loadSound(char *path);

void initSDL(void);
void processKeyboardInput(SDL_Event *eventPtr, bool *running);

void updateObjs(void);
void handleCollisions(void);
void drawSprites(void);

#endif
