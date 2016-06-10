#ifndef MENU_H
#define MENU_H

#include <stdlib.h>
#include <stdio.h>
#include "Structs.h"
#include "GameEdit.h"

void initMenuObj(GameObject *menuObj);
void drawMenu(GameObject *menuObj);
void updateMenu(GameObject *menuObj);
void incrementMenu(GameObject *menuObj);
void selectMenu(GameObject * menuObj);

#endif
