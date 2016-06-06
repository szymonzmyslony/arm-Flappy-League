#ifndef TMINUS_H
#define TMINUS_H

SDL_Surface *getConsoleScreen(void);
void initSDL(void);
void processKeyboardInput(SDL_Event *eventPtr, SDL_Surface *screen,
                          bool *running);

#endif
