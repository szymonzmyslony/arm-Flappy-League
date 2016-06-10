#include "Menu.h"

#define index     menuObj->v1.i
#define numElems  menuObj->v2.i
void initMenu(GameObject *menuObj) {
  initMenu->index = 0;
  initMenu->numElems = 2;
  /* first 'element' is new game 
  /  second 'element' is toggle sound */

  initMenu->draw = &drawMenu;
  initMenu->update = &updateMenu;
}

void drawMenu(GameObject *menuObj) {
  return;
}

void updateMenu(GameObject *menuObj) {
  return;
}
