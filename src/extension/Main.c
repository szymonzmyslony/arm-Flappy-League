#include "Main.h"

#define FPS 60
#define SPF SECOND / FPS
#define COEFF_OF_RESTITUTION 1

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
  // -- Initialise Pins
  #ifdef RPI
  initPins();
  #endif
  // Get the console's screen for drawing
  screen = getConsoleScreen();
  // Set the window bar data. For non-console use only.
  SDL_WM_SetCaption("window mcwindowface", NULL);

  // -- Load Images
  surf_bg = loadImage("gfx/Crowd.png");
  surf_ball = loadImage("gfx/Ball.png");
  surf_goal = loadImage("gfx/Goal.png");
  surf_bird1 = loadImage("gfx/KingClumsy.png");
  surf_bird2 = loadImage("gfx/SirFlappy.png");
  surf_bird3 = loadImage("gfx/FighterJumpy.png");
  surf_bird4 = loadImage("gfx/PunkSilly.png");
  surf_datboi = loadImage("gfx/DatBoi.png");
  surf_flappybird = loadImage("gfx/FlappyBird.png");
  surf_scoring = loadImage("gfx/scoring.png");

  // -- Load Sounds
  music_crowd = loadMusic("sound/stadium_noise.wav");
  sound_whistle = loadSound("sound/whistle.wav");
  sound_kick1 = loadSound("sound/kick1.wav");
  sound_kick2 = loadSound("sound/kick1.wav");
  sound_kick3 = loadSound("sound/kick1.wav");
  sound_kick4 = loadSound("sound/kick1.wav");
  sound_goal = loadSound("sound/goal.wav");

  // -- Initialise Game Variables

  gObjs = (GameObject**)calloc(MAX_OBJECTS, sizeof(GameObject*));
  if(gObjs == NULL) {
    fprintf(stderr, "Error allocating memory\n");
    exit(EXIT_FAILURE);
  }
  for(int i = 0; i < MAX_OBJECTS; i++) {
    gObjs[i] = (GameObject*)calloc(1, sizeof(GameObject));
    if(gObjs[i] == NULL) {
      fprintf(stderr, "Error allocating memory\n");
      exit(EXIT_FAILURE);
    }
  }

  Mix_PlayMusic(music_crowd, -1);

  initGame();

  // -- Initialise Loop variables
  // A union capable of holding all input events
  SDL_Event event;
  bool running = true;
  bool bgDrawn = false;
  uint32_t lastUpdate = SDL_GetTicks();

  // Game Loop
  while(running) {
    // Process SDL keyboard input events. For non-Pi only.
    processKeyboardInput(&event, &running);
    // Process GPIO pins
    updateButtonsStatus();

    // Draw background between frames, too intensive
    if(!bgDrawn) {
      SDL_BlitSurface(surf_bg, NULL, screen, NULL);
      //SDL_FillRect(screen, NULL,
      //  SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

      bgDrawn = true;
    }

    uint32_t now = SDL_GetTicks();
    if(now - lastUpdate > SPF) {
      lastUpdate = now;

      //Convert input to real effects
      handleButtonStatus();

      // Update each gameObject that has an update function
      updateObjs();
      handleCollisions();
      // Draw all gameObjects onto screen that have a sprite
      drawSprites();

      // Swap the back buffer's contents with the front buffer's
      // Update the screen
      SDL_Flip(screen);
      bgDrawn = false;
    }
  }

  // Cleanup
  for(int i = 0; i < MAX_OBJECTS; i++) {
    if(gObjs[i] != NULL) {
      free(gObjs[i]);
    }
  }
  if(gObjs != NULL) {
    free(gObjs);
  }

  // Free Sound Resources
  Mix_FreeMusic(music_crowd);

  Mix_FreeChunk(sound_whistle);
  Mix_FreeChunk(sound_kick1);
  Mix_FreeChunk(sound_kick2);
  Mix_FreeChunk(sound_kick3);
  Mix_FreeChunk(sound_kick4);
  Mix_FreeChunk(sound_goal);

  //Free Graphical Resources
  SDL_FreeSurface(screen);
  SDL_FreeSurface(surf_bg);
  SDL_FreeSurface(surf_ball);
  SDL_FreeSurface(surf_goal);
  SDL_FreeSurface(surf_flappybird);
  SDL_FreeSurface(surf_datboi);
  SDL_FreeSurface(surf_bird1);
  SDL_FreeSurface(surf_bird2);
  SDL_FreeSurface(surf_bird3);
  SDL_FreeSurface(surf_bird4);
  SDL_FreeSurface(surf_scoring);

  // Release Initialised SDL Systems
  Mix_CloseAudio();
  Mix_Quit();
  IMG_Quit();
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
          resolveCollisionAdvanced(&(circObj->v2.vec), &(gObjs[j]->v2.vec),
                                   1, 1,
                                   &(circObj->v1.vec), &(gObjs[j]->v1.vec),
                                  COEFF_OF_RESTITUTION);
        }
      }

    }
  }
  // Circle - Other collision
  for(int i = 0; i < MAX_OBJECTS; i++) {
    for(int j = 0; j < MAX_OBJECTS; j++) {
      GameObject *circObj = gObjs[i];
      if(circObj != NULL && circObj->colliderType == COL_CIRCLE
         && gObjs[j] != NULL) {
        switch(gObjs[j]->colliderType) {
          case COL_BOX:
            break;

          case COL_NET:
            if(gObjs[j]->v3.func != NULL
               && circleNetCollided(circObj, gObjs[j])) {
              gObjs[j]->v3.gFunc(circObj);
            }
            break;

          default:
            break;
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
              buttonDownP1Left = true;
              break;

            case SDLK_x:
              buttonDownP1Right = true;
              break;

            case SDLK_n:
              buttonDownP2Left = true;
              break;

            case SDLK_m:
              buttonDownP2Right = true;
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
                                          0, SDL_SWSURFACE);
                                          //SDL_HWSURFACE | SDL_DOUBLEBUF );
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
  if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
  {
    fprintf(stderr, "Error initialising Mixer");
    exit(EXIT_FAILURE);
  }
}

inline void initPins(void){
  if(wiringPiSetup() == -1) {
      fprintf(stderr, "Failed to setup controllers");
      exit(EXIT_FAILURE);
  }

  for(int i = LeftFirstPlayer; i<=LeftSecondPlayer; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
}

/** Attempts to load an image. The image is unoptimised.
*/
SDL_Surface *loadImage(char *path) {
  SDL_Surface *formattedSurface = NULL;
  SDL_Surface *loadedSurface = IMG_Load(path);

  if(loadedSurface == NULL) {
    fprintf(stderr, "Error loading %s: %s\n", path, IMG_GetError());
    exit(EXIT_FAILURE);
  }

  formattedSurface = SDL_DisplayFormatAlpha(loadedSurface);
  if(formattedSurface == NULL) {
    fprintf(stderr, "Error formatting %s\n", path);
    return loadedSurface;
  }

  SDL_FreeSurface(loadedSurface);
  return formattedSurface;
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
