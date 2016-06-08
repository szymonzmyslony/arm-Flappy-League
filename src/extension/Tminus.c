#include "Tminus.h"

#define FPS 60
#define SPF 1000 / FPS

enum Display { WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 540 };

//TODO consider SDL 2.0

/** SDL_main is used for Windows / Mac, we will just use Linux
 * DARWIN is MAC OS X
 */
#if defined (DARWIN) || defined (WIN32)
int SDL_main(int argc, char **argv)
#else
int main(int argc, char **argv) {
#endif
  // -- Initialise SDL, Graphical Interfaces
  initSDL();
  // Get the console's screen for drawing
  screen = getConsoleScreen();
  // Set the window bar data. For non-console use only.
  SDL_WM_SetCaption("window mcwindowface", NULL);

  // -- Load Images
  SDL_Surface *surf_datboi = loadImage("gfx/DatBoi.png");
  SDL_Surface *surf_flappybird = loadImage("gfx/FlappyBird.png");
  SDL_Surface *surf_ball = loadImage("gfx/Ball.png");
  SDL_Surface *surf_bg = loadImage("gfx/Crowd.png");

  // -- Initialise Game Variables

  gObjs = (GameObject**)calloc(MAX_OBJECTS, sizeof(GameObject*));
  if(gObjs == NULL) {
    fprintf(stderr, "Error allocating memory\n");
    exit(EXIT_FAILURE);
  }

  //TODO remove test code
  gObjs[0] = initCircleObj(32, 300, 300, 10,  0);
  gObjs[1] = initCircleObj(32, 100, 300, 0,  0);
  setSprite(gObjs[0], surf_ball);
  setSprite(gObjs[1], surf_datboi);

  // gObjs[2] = initCircleObj(32, 300, 300, 1, -1);
  // gObjs[3] = initCircleObj(32, 300, 300, 1,  0);
  // setSprite(gObjs[2], surf_datboi);
  // setSprite(gObjs[3], surf_datboi);
  //
  // gObjs[4] = initCircleObj(32, 300, 300, -1, -1);
  // gObjs[5] = initCircleObj(32, 300, 300, -1,  0);
  // setSprite(gObjs[4], surf_datboi);
  // setSprite(gObjs[5], surf_datboi);
  //
  // gObjs[6] = initCircleObj(32, 300, 300, 0, -1);
  // gObjs[7] = initCircleObj(32, 300, 300, 0,  1);
  // setSprite(gObjs[6], surf_datboi);
  // setSprite(gObjs[7], surf_datboi);
  //
  // gObjs[8] = initCircleObj(32, 300, 300, -1,  1);
  // setSprite(gObjs[8], surf_datboi);

  //gObjs[9] = initTimerObj(999999, true, &updateTimerConstant, &applyAllGravity);

  // -- Initialise Loop variables
  // A union capable of holding all input events
  SDL_Event event;
  bool running = true;
  uint32_t lastUpdate = SDL_GetTicks();

  // Game Loop
  while(running) {
    SDL_BlitSurface(surf_bg, NULL, screen, NULL);

    //SDL_FillRect(screen, NULL,
    //  SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

    // Process SDL keyboard input events. For non-Pi only.
    processKeyboardInput(&event, &running);

    //TODO Process GPIO pins

    uint32_t now = SDL_GetTicks();
    if(now - lastUpdate > SPF) {
      lastUpdate = now;

      // Update each gameObject that has an update function
      updateObjs();
      handleCollisions();
      // Draw all gameObjects onto screen that have a sprite
      drawSprites();

      // Swap the back buffer's contents with the front buffer's
      // Update the screen
      SDL_Flip(screen);
    }
  }

  // Cleanup
  free(gObjs);
  SDL_Quit();

  printf("Exiting T-\n");

  return 0;
}

inline void updateObjs() {
  for(int i = 0; i < MAX_OBJECTS; i++) {
    if(gObjs[i] != NULL && gObjs[i]->update != NULL) {
      gObjs[i]->update(gObjs[i]);
    }
  }
}

inline void handleCollisions(void) {
  // Circle - Circle collision
  for(int i = 0; i < MAX_OBJECTS; i++) {
    for(int j = i + 1; j < MAX_OBJECTS; j++) {
      GameObject *circObj = gObjs[i];
      if(circObj != NULL && circObj->colliderType == COL_CIRCLE
         && gObjs[j] != NULL && gObjs[j]->colliderType == COL_CIRCLE) {
        if(circlesCollided(circObj, gObjs[j])) {
          resolveCollision(&(circObj->v2.vec), &(gObjs[j]->v2.vec), 1, 1, 0.05);
        }
      }
    }
  }
}

inline void drawSprites(void) {
  for(int i = 0; i < MAX_OBJECTS; i++) {
    if(gObjs[i] != NULL && gObjs[i]->draw != NULL) {
      gObjs[i]->draw(gObjs[i]);
    }
  }
}

/** Processes each keyboard event that occurred. For non-Pi testing purposes.
* May change the screen, and cause the game to stop running.
*/
inline void processKeyboardInput(SDL_Event *eventPtr, bool *running) {
  while (SDL_PollEvent(eventPtr)) {
    switch (eventPtr->type) {
      // Closable via x window button if run outside of a console.
      case SDL_QUIT:
        *running = false;
        break;
      // Key down events - when a key is first pressed down (not held)
      case SDL_KEYDOWN:
          switch (eventPtr->key.keysym.sym) {
            case SDLK_ESCAPE:
              *running = false;
              break;

            case SDLK_BACKSPACE:
              SDL_FillRect(screen, NULL,
                SDL_MapRGB(screen->format, 0xFF, 0x00, 0xFF));
              break;

            case SDLK_SPACE:
              SDL_FillRect(screen, NULL,
                SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));
              break;

            default:
              break;
          }
        break;

      default:
        break;
    }
  }
}

/** Get the framebuffer, with a set width and height, and using the native bits
* per pixel.
* Flags are for double buffering and getting the surface from hardware (GPU
* memory). Pis do not give a HWSURFACE. Pis seem to do better without a double
* buffer
*/
inline SDL_Surface *getConsoleScreen(void) {
  SDL_Surface *screenPtr = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT,
                                          0, SDL_HWSURFACE);// | SDL_DOUBLEBUF );
  if(screenPtr == NULL) {
    fprintf(stderr, "Error setting SDL Video Mode\n");
    exit(EXIT_FAILURE);
  }

  return screenPtr;
}

/** Initialise SDL using video, audio and the timer.
*/
inline void initSDL(void) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
    fprintf(stderr, "Error initialising SDL\n");
    exit(EXIT_FAILURE);
  }

  // Load support for PNG
  int imgFlags = IMG_INIT_PNG;
  if(!(IMG_Init(imgFlags) & imgFlags)) {
    fprintf(stderr, "Error initialising SDL_image: %s\n", IMG_GetError());
    exit(EXIT_FAILURE);
  }
}

void initMenu(void) {

}

/**
*/
SDL_Surface *loadImage(char *path)
{
  SDL_Surface *optimizedSurface = NULL;
  SDL_Surface *loadedSurface = IMG_Load(path);

  if(loadedSurface == NULL) {
    fprintf(stderr, "Error loading %s: %s\n", path, IMG_GetError());
    exit(EXIT_FAILURE);
  }

  return loadedSurface;
}
