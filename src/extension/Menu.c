#include "Menu.h"

#define index     v1.i
#define numElems  v2.i

enum MenuOptions { START_GAME, SOUND_SELECT, NUMBER_OF_OPTIONS_M };
enum EndMenuOptions { NEW_GAME, RETURN_TO_MAIN, NUMBER_OF_OPTIONS_E };

void initMenuObj(GameObject *menuObj) {
  menuObj->index = 0;
  menuObj->numElems = NUMBER_OF_OPTIONS_M;
  /* First 'element' is new game 
  /  Second 'element' is toggle sound */
 
  menuObj->draw = &drawMenu;
  menuObj->update = &updateMenu;
}

void initEndScreenObj(GameObject *endObj) {
  endObj->index = 0;
  endObj->numElems = NUMBER_OF_OPTIONS_E;
  
  // First and only element is new game

  //TODO update these function pointers
  endObj->draw = &drawEndMenu;
  endObj->update = &updateMenu;
}

void drawMenu(GameObject *menuObj) {
  if (menuObj->sprite == NULL) {
    return;
  }
  int screenX = 387;
  int screenY = 130;
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
  int screenX = 387;
  int screenY = 300;
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
