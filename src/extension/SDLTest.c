#include <SDL/SDL.h>
#include <stdbool.h>

enum Display { WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 720 };

/** SDL_main is used for Windows / Mac, we will just use Linux
 * DARWIN is MAC OS X
 */
#if defined (DARWIN) || defined (WIN32)
  int SDL_main(int argc, char **argv)
#else
  int main(int argc, char **argv) {
#endif
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO |
                 SDL_INIT_TIMER) != 0) {
      fprintf(stderr, "%s\n", "Error initialising SDL");
      exit(EXIT_FAILURE);
    }

    // Use default bits per pixel with 0
    SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT,
                                            0, SDL_HWSURFACE | SDL_DOUBLEBUF );
    if(screen == NULL) {
      fprintf(stderr, "%s\n", "Error setting SDL Video Mode");
      exit(EXIT_FAILURE);
    }

    // No window icon, so NULL
    SDL_WM_SetCaption("window mcwindowface", NULL);

    SDL_Event event;
    bool quit = false;
    // now we loop until the quit flag is set to true
    while(!quit) {
     // process SDL events, in this case we are looking for keys
      while ( SDL_PollEvent(&event) ) {
        switch (event.type) {
            // this is the window x being clicked.
          case SDL_QUIT : quit = true; break;
            // now we look for a keydown event
          case SDL_KEYDOWN:
              switch( event.key.keysym.sym ) {
                // if it's the escape key quit
                case SDLK_ESCAPE:
                  quit = true;
                  break;

                case SDLK_BACKSPACE:
                  SDL_FillRect(screen, NULL,
                    SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF) );
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
  }
