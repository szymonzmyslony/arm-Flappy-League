#include "Tminus.h"

#define FPS 60
#define SPF 1000 / FPS
#define COEFF_OF_RESTITUTION 1

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

  // -- Load Sounds
  Mix_Music *music_crowd = loadMusic("sound/stadium_noise.wav");
  Mix_Chunk *sound_goal = loadSound("sound/goal.wav");
  Mix_Chunk *sound_kick1 = loadSound("sound/kick1.wav");
  Mix_Chunk *sound_kick2 = loadSound("sound/kick1.wav");
  Mix_Chunk *sound_kick3 = loadSound("sound/kick1.wav");
  Mix_Chunk *sound_kick4 = loadSound("sound/kick1.wav");

  // -- Initialise Game Variables

  gObjs = (GameObject**)calloc(MAX_OBJECTS, sizeof(GameObject*));
  if(gObjs == NULL) {
    fprintf(stderr, "Error allocating memory\n");
    exit(EXIT_FAILURE);
  }

  Mix_PlayMusic(music_crowd, -1);

  //TODO remove test code
  gObjs[0] = initCircleObj(32, 100, 300, 0,  0);
  setSprite(gObjs[0], surf_datboi);

  gObjs[1] = initCircleObj(32, screen->w - 100, 300, 0,  0);
  setSprite(gObjs[1], surf_flappybird);

  gObjs[2] = initCircleObj(32, screen->w/2, 300, 0, 0);
  setSprite(gObjs[2], surf_ball);

  gObjs[9] = initTimerObj(UINT32_MAX, true, &updateTimerConstant, &applyAllGravity);
  gObjs[10] = initTimerObj(UINT32_MAX, true, &updateTimerConstant, &applyAllAirResistance);

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
  // Free Sound Resources
  Mix_FreeMusic(music_crowd);
  Mix_FreeChunk(sound_goal);
  Mix_FreeChunk(sound_kick1);
  Mix_FreeChunk(sound_kick2);
  Mix_FreeChunk(sound_kick3);
  Mix_FreeChunk(sound_kick4);

  //Free Graphical Resources
  SDL_FreeSurface(screen);
  SDL_FreeSurface(surf_bg);
  SDL_FreeSurface(surf_ball);
  SDL_FreeSurface(surf_flappybird);
  SDL_FreeSurface(surf_datboi);

  // Release Initialised SDL Systems
  Mix_CloseAudio();
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
          resolveCollision(&(circObj->v2.vec), &(gObjs[j]->v2.vec), 1, 1,
                           COEFF_OF_RESTITUTION);
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

            case SDLK_z:
              gObjs[0]->v2.vec.x = - 5.0;
              gObjs[0]->v2.vec.y = -10.0;
              break;

            case SDLK_x:
              gObjs[0]->v2.vec.x =   5.0;
              gObjs[0]->v2.vec.y = -10.0;
              break;

            case SDLK_n:
              gObjs[1]->v2.vec.x = - 5.0;
              gObjs[1]->v2.vec.y = -10.0;
              break;

            case SDLK_m:
              gObjs[1]->v2.vec.x =   5.0;
              gObjs[1]->v2.vec.y = -10.0;
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

  //Initialise Sound Mixer
  if(Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
  {
    fprintf(stderr, "Error initialising Mixer");
    exit(EXIT_FAILURE);
  }
}

void initMenu(void) {

}

/** Attempts to load an image. The image is unoptimised.
*/
SDL_Surface *loadImage(char *path) {
  SDL_Surface *optimizedSurface = NULL;
  SDL_Surface *loadedSurface = IMG_Load(path);

  if(loadedSurface == NULL) {
    fprintf(stderr, "Error loading %s: %s\n", path, IMG_GetError());
    exit(EXIT_FAILURE);
  }

  return loadedSurface;
}

/** Attempts to load a music file, in .wav format
*/
Mix_Music *loadMusic(char *path) {
  Mix_Music *music = Mix_LoadMUS(path);
  if(music == NULL) {
    fprintf(stderr, "Error loading %s\n", path);
    exit(EXIT_FAILURE);
  }
  return music;
}

/** Attempts to load a sound file, in .wav format
*/
Mix_Chunk *loadSound(char *path) {
  Mix_Chunk *sound = Mix_LoadWAV(path);
  if(sound == NULL) {
    fprintf(stderr, "Error loading %s\n", path);
    exit(EXIT_FAILURE);
  }
  return sound;
}
