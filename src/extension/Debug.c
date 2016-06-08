#include "Debug.h"
#define DEBUG_COLOUR 0xAACCCCAA
// Handy code for drawing straight onto the screen, for debugging purposes.
/** The screen must be locked before calling.
 */
void debugDrawPixel(int x, int y, uint32_t pixel) {
  if(x < 0 || x > screen->w || y < 0 || y > screen->h) {
    return;
  }

  int bpp = screen->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to set */
  Uint8 *p = (uint8_t *)screen->pixels + y * screen->pitch + x * bpp;

  switch(bpp) {
  case 1:
    *p = pixel;
    break;

  case 2:
    *(uint16_t *)p = pixel;
    break;

  case 3:
    if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
        p[0] = (pixel >> 16) & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = pixel & 0xff;
    } else {
        p[0] = pixel & 0xff;
        p[1] = (pixel >> 8) & 0xff;
        p[2] = (pixel >> 16) & 0xff;
    }
    break;

  case 4:
    *(uint32_t *)p = pixel;
    break;
  }
}

void debugDrawCircle(int x, int y, int r) {
  for(int dy = -r; dy <= r; dy++) {
    for(int dx = -r; dx <= r; dx++) {
      if(dx * dx + dy * dy <= r * r) {
        debugDrawPixel(x + dx, y + dy, DEBUG_COLOUR);
      }
    }
  }
}
