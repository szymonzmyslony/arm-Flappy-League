#ifndef STRUCTS_H
#define STRUCTS_H
#include <stdbool.h>
#include <stdint.h>
#include <SDL/SDL.h>

typedef struct vector {
  float x;
  float y;
} Vector;

/** A union that represents a generic variable, allowing the GameObject struct
* to be more versatile and extremely generic, at the cost of taking up more
* memory.
*/
typedef union var {
  void (*func)(void);
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
typedef struct gameObject GameObject;
struct gameObject {
  Var v1, v2, v3, v4, v5, v6;
  // The sprite drawn, if it is not NULL, and the function called to draw it
  SDL_Surface *sprite;
  void (*draw)(GameObject *gObj);
  // A function called every frame, if it is not NULL
  void (*update)(GameObject *gObj);
};

// Global Variables

SDL_Surface *screen;
GameObject **gObjs;

#endif
