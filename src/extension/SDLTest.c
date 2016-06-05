#include <SDL/SDL.h>
#include <stdbool.h>

enum Display { WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 720 };

/** Get the framebuffer, with a set width and height, and using the native bits
* per pixel.
* Flags are for double buffering and getting the surface from hardware (GPU
* memory)
*/
SDL_Surface *getConsoleScreen(void) {
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
  void initSDL(void) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
      fprintf(stderr, "%s\n", "Error initialising SDL");
      exit(EXIT_FAILURE);
    }
  }
/** SDL_main is used for Windows / Mac, we will just use Linux
 * DARWIN is MAC OS X
 */
#if defined (DARWIN) || defined (WIN32)
int SDL_main(int argc, char **argv)
#else
int main(int argc, char **argv) {
#endif
  initSDL();

  // Get the console's screen for drawing
  SDL_Surface* screen = getConsoleScreen();

  // No window icon, so NULL
  SDL_WM_SetCaption("window mcwindowface", NULL);

  // A union capable of holding all input events
  SDL_Event event;
  bool quit = false;
  // now we loop until the quit flag is set to true
  while(!quit) {
   // Process SDL input events, one at a time
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        // Closable via x window button if run outside of a console.
        case SDL_QUIT :
          quit = true;
          break;
        // Key down events - when a key is first pressed down (not held)
        case SDL_KEYDOWN:
            switch( event.key.keysym.sym ) {
              case SDLK_ESCAPE:
                quit = true;
                break;

              case SDLK_BACKSPACE:
                SDL_FillRect(screen, NULL,
                  SDL_MapRGB(screen->format, 0xFF, 0x00, 0xFF) );
                break;

              case SDLK_SPACE:
                SDL_FillRect(screen, NULL,
                  SDL_MapRGB(screen->format, 0x00, 0x00, 0x00) );
                break;

              default:
                break;
            }
          break;

        default:
          break;
      }
    }

    SDL_Flip(screen);
  }

  SDL_Quit();

  return 0;
}
