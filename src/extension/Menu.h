#ifndef MENU_H
#define MENU_H

#include <stdlib.h>
#include <stdio.h>
#include "Common.h"
#include "GameEdit.h"

void initMenuObj(GameObject *menuObj);
void initEndScreen(GameObject *endObj);
void drawMenu(GameObject *menuObj);
void drawEndMenu(GameObject *endObj);
void updateMenu(GameObject *menuObj);
void incrementMenu(GameObject *menuObj);
void selectMenu(GameObject * menuObj);
void selectEndMenu(GameObject * menuObj);

#endif
