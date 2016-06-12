#include "Menu.h"

#define mx        v3.vec.x
#define my        v3.vec.y
#define index     v1.i
#define numElems  v2.i

enum MenuOptions { START_GAME, SOUND_SELECT, NUMBER_OF_OPTIONS_M };
enum EndMenuOptions { NEW_GAME, RETURN_TO_MAIN, NUMBER_OF_OPTIONS_E };

void initMenuObj(GameObject *menuObj) {
  menuObj->objType = OBJ_MENU;
  menuObj->index = 0;
  menuObj->mx = 387;
  menuObj->my = 130;
  menuObj->numElems = NUMBER_OF_OPTIONS_M;
  /* First 'element' is new game
  /  Second 'element' is toggle sound */

  menuObj->draw = &drawMenu;
  menuObj->update = &updateMenu;
}

void initEndScreenObj(GameObject *endObj) {
  endObj->objType = OBJ_MENU;
  endObj->index = 0;
  endObj->numElems = 2;
  endObj->mx = 387;
  endObj->my = 300;

  // First and only element is new game

  //TODO update these function pointers
  endObj->draw = &drawEndMenu;
  endObj->update = &updateMenu;
}

void drawMenu(GameObject *menuObj) {
  if (menuObj->sprite == NULL) {
    return;
  }

  int screenX = menuObj->mx;
  int screenY = menuObj->my;

  int spriteNo;
  for (int i = 0; i < menuObj->numElems; i++) {
    if (i == menuObj->index) {
      spriteNo = 1;
    } else {
      spriteNo = 0;
    }
    if (i == SOUND_SELECT) {
      if (getSoundState()){
        spriteNo += 2;
      }
    }

    animate(menuObj->sprite, i, spriteNo, 250, 50, screenX, screenY);
    screenY += 50;
  }
  return;
}

void drawEndMenu(GameObject *endObj) {
  if (endObj->sprite == NULL) {
    return;
  }

  int screenX = endObj->mx;
  int screenY = endObj->my;

  int spriteNo;
  for (int i = 0; i < endObj->numElems; i++) {
    if (i == endObj->index) {
      spriteNo = 1;
    } else {
      spriteNo = 0;
    }

    animate(endObj->sprite, i, spriteNo, 250, 50, screenX, screenY);
    screenY += 50;
  }
  return;
}

void updateMenu(GameObject *menuObj) {
  return;
}

void incrementMenu(GameObject *menuObj) {
  menuObj->index = (menuObj->index + 1) % menuObj->numElems;
  return;
}

void selectMenu(GameObject *menuObj) {
  switch (menuObj->index) {
    case START_GAME:
      initGame();
      break;
    case SOUND_SELECT:
      toggleSound();
      break;
    default:
      fprintf(stderr, "Error: Unsupported menu option\n");
  }
}

void selectEndMenu(GameObject *menuObj) {
  switch (menuObj->index) {
    case NEW_GAME:
      initGame();
      break;
    case RETURN_TO_MAIN:
      initMenu();
      break;
    default:
      fprintf(stderr, "Error: Unsupported menu option\n");
  }
}
