#ifndef DEBUG_H
#define DEBUG_H

#include <SDL/SDL.h>
#include <stdint.h>
#include "Structs.h"

void debugDrawPixel(int x, int y, uint32_t pixel);
void debugDrawCircle(int x, int y, int r);

#endif
