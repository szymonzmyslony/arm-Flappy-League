#ifndef COMMON_H
#define COMMON_H
#include <stdbool.h>
#include <stdint.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#define SECOND 1000
#define MATCH_LENGTH 120 * SECOND

enum Display { WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 540 };
enum Game { MAX_OBJECTS = 20 };

/** These IDs also correspond to the drawing order
*/
enum ids {
  SCOREBOARD1 = 0,
  SCOREBOARD2 = 1,
  TIMEBOARD = 2,
  PLAYER1 = 3,
  PLAYER2 = 4,
  BALL = 5,
  GOAL1 = 6,
  GOAL2 = 7,
  MAINMENU = 8,
  ENDSCREEN = 9,
  GRAVITY_TIMER = 10,
  AIR_RES_TIMER = 11,
  WHISTLE_TIMER = 12,
  MATCH_TIMER = 13,
  TITLE = 14,
  STATS = 15
};

typedef struct vector {
  float x;
  float y;
} Vector;

enum objTypes {
  COL_CIRCLE = 1,
  COL_LINE   = 2,
  COL_NET    = 3,
  COL_BOX    = 4,
  OBJ_MENU   = 5,
};

typedef struct gameObject GameObject;
/** A union that represents a generic variable, allowing the GameObject struct
* to be more versatile and extremely generic, at the cost of taking up more
* memory.
*/
typedef union var {
  void (*gFunc)(GameObject*);
  void (*func)(void);
  GameObject *g;
  Vector vec;
  uint32_t u;
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
struct gameObject {
  uint8_t objType;
  Var v1, v2, v3, v4, v5, v6;
  SDL_Surface *sprite;
  void (*draw)(GameObject *gObj);
  // A function called every frame, if it is not NULL
  void (*update)(GameObject *gObj);
};

// Global Variables

SDL_Surface *screen;
GameObject **gObjs;
bool buttonDownP1Left;
bool buttonDownP1Right;
bool buttonDownP2Left;
bool buttonDownP2Right;

enum spriteTileSizes {
  SURF_NUMBERS_TWIDTH = 22,
  SURF_NUMBERS_HEIGHT = 29,
  SURF_TIMING_TWIDTH = 14,
  SURF_SCORING_TWIDTH = 41,
  SURF_BALL_TWIDTH = 64,
  SURF_BIRD_TWIDTH = SURF_BALL_TWIDTH
};

// Resources
SDL_Surface *surf_datboi;
SDL_Surface *surf_bird1;
SDL_Surface *surf_bird2;
SDL_Surface *surf_bird3;
SDL_Surface *surf_bird4;
SDL_Surface *surf_ball;
SDL_Surface *surf_goal;
SDL_Surface *surf_flaps;
SDL_Surface *surf_kicks;
SDL_Surface *surf_bg;
SDL_Surface *surf_main_menu;
SDL_Surface *surf_scoring;
SDL_Surface *surf_timing;
SDL_Surface *surf_end_menu;
SDL_Surface *surf_title;
SDL_Surface *surf_numbers;

Mix_Music *music_crowd;

Mix_Chunk *sound_whistle;
Mix_Chunk *sound_kick1;
Mix_Chunk *sound_kick2;
Mix_Chunk *sound_kick3;
Mix_Chunk *sound_kick4;
Mix_Chunk *sound_goal;

#endif
