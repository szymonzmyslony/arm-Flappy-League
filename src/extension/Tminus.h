#ifndef TMINUS_H
#define TMINUS_H

SDL_Surface *getConsoleScreen(void);
SDL_Surface *loadImage(char *path);
void initSDL(void);
void processKeyboardInput(SDL_Event *eventPtr, bool *running);

#endif
