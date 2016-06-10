#include "Menu.h"

#define index     v1.i
#define numElems  v2.i

void initMenuObj(GameObject *menuObj) {
  menuObj->index = 0;
  menuObj->numElems = 2;
  /* First 'element' is new game 
  /  Second 'element' is toggle sound */
 
  menuObj->draw = &drawMenu;
  menuObj->update = &updateMenu;
}

void initEndScreenObj(GameObject *endObj) {
  endObj->index = 0;
  endObj->numElems = 2;
  
  // First and only element is new game

  //TODO update these function pointers
  endObj->draw = &drawMenu;
  endObj->update = &updateMenu;
}

void drawMenu(GameObject *menuObj) {
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
    case 0:
      initGame();
      break;
    case 1:
      toggleSound();
      break;
    default:
      fprintf("stderr", "Error: Unsupported menu option\n");
  }
}

void selectEndMenu(GameObject *menuObj) {
  switch (menuObj->index) {
    case 0:
      initGame();
      break;
    case 1:
      initMenu();
      break;
    default:
      fprintf("stderr", "Error: Unsupported menu option\n");
  }
}
