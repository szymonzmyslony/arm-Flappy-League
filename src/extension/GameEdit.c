#include "GameEdit.h"

void initMenu(void) {
  initSetup();

}

void initGame(void) {
  initSetup();
}

void initSetup(void) {
  gObjs[0] = initCircleObj(32, 100, 300, 0,  0);
  setSprite(gObjs[0], surf_datboi);

  gObjs[1] = initCircleObj(32, screen->w - 100, 300, 0,  0);
  setSprite(gObjs[1], surf_flappybird);

  gObjs[2] = initCircleObj(32, screen->w/2, 300, 0, 0);
  setSprite(gObjs[2], surf_ball);

  gObjs[9] = initTimerObj(UINT32_MAX, true, &updateTimerConstant, &applyAllGravity);
  gObjs[10] = initTimerObj(UINT32_MAX, true, &updateTimerConstant, &applyAllAirResistance);
}
