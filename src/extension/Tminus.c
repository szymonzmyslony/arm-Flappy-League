#include <SDL/SDL.h>
#include <stdbool.h>
#include "Tminus.h"

enum Display { WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 720 };

/** A union that represents a generic variable, allowing the GameObject struct
* to be more versatile and useful in a variety of ways.
*/
typedef union var {
  float f;
  void *p;
  bool b;
  int i;
} Var;

/** A struct that represents every drawn object and every function that is run
* in each frame. The variables are used in a way that is consistent to their
* draw and update function pointers.
*   For example, a countdown may use v1 as a float and v2 as the halfway point
* to switch the sides of each player in a football game when update() is
* called.
*/
typedef struct gameObject GameObject;
struct gameObject {
  Var v1, v2, v3, v4, v5, v6;
  // The sprite drawn, if it is not NULL, and the function called to draw it
  SDL_Surface *sprite;
  void (*draw)(GameObject *gObj);
  // A function called every frame, if it is not NULL
  void (*update)(GameObject *gObj);
};

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
  SDL_Surface *screen = getConsoleScreen();
  // Set the window bar data. For non-console use only.
  SDL_WM_SetCaption("window mcwindowface", NULL);

  // -- Initialise loop variables
  // A union capable of holding all input events
  SDL_Event event;
  bool running = true;

  // Game Loop
  while(running) {
    // Process SDL input events, one at a time. For non-Pi only.
    processKeyboardInput(&event, screen, &running);

    /*// Update each gameObject
    for() {
      gObj->update(gObj);
    }

    // Draw all gameObjects onto screen
    for() {
      gObj->draw(gObj);
    }*/

    // Swap the back buffer's contents with the front buffer's
    // Update the screen
    SDL_Flip(screen);
  }

  SDL_Quit();

  return 0;
}

inline void processKeyboardInput(SDL_Event *eventPtr, SDL_Surface *screen,
                                 bool *running) {
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
* memory). Pis do not give a HWSURFACE.
*/
inline SDL_Surface *getConsoleScreen(void) {
  SDL_Surface *screenPtr = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT,
                                          0, SDL_HWSURFACE | SDL_DOUBLEBUF );
  if(screenPtr == NULL) {
    fprintf(stderr, "%s\n", "Error setting SDL Video Mode");
    exit(EXIT_FAILURE);
  }

  return screenPtr;
}

/** Initialise SDL using video, audio and the timer.
*/
inline void initSDL(void) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
    fprintf(stderr, "%s\n", "Error initialising SDL");
    exit(EXIT_FAILURE);
  }
}
