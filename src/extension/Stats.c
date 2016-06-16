#include "Stats.h"

/** Stats are not drawn when this is called, use showStats to do so.
*/
void initStatsObj(GameObject *statsObj) {
  clearObj(statsObj);
  statsObj->update = &updateStats;
}

void showStats(GameObject *statsObj) {
  statsObj->draw = drawStats;
}

void drawStats(GameObject *statsObj) {
  // Draw Stats for P1
  SDL_Rect kP1 = {
    .x = STATS_KICK_OFFSET_X1 + 10,
    .y = STATS_KICK_OFFSET_Y - 10,
    .w = 0,
    .h = 0
  };
  SDL_BlitSurface(surf_kicks, NULL, screen, &kP1);
  drawNum(statsObj->v1.vec.x, STATS_KICK_OFFSET_X1 - 10, STATS_KICK_OFFSET_Y);

  SDL_Rect fP1 = {
    .x = STATS_FLAP_OFFSET_X1 + 10,
    .y = STATS_FLAP_OFFSET_Y - 10,
    .w = 0,
    .h = 0
  };
  SDL_BlitSurface(surf_flaps, NULL, screen, &fP1);
  drawNum(statsObj->v2.vec.x, STATS_FLAP_OFFSET_X1 - 10, STATS_FLAP_OFFSET_Y);

  // Draw Stats for P2
  SDL_Rect kP2 = {
    .x = STATS_KICK_OFFSET_X2 + 10,
    .y = STATS_KICK_OFFSET_Y - 10,
    .w = 0,
    .h = 0
  };
  SDL_BlitSurface(surf_kicks, NULL, screen, &kP2);
  drawNum(statsObj->v1.vec.y, STATS_FLAP_OFFSET_X2 - 10, STATS_KICK_OFFSET_Y);

  SDL_Rect fP2 = {
    .x = STATS_FLAP_OFFSET_X2 + 10,
    .y = STATS_FLAP_OFFSET_Y - 10,
    .w = 0,
    .h = 0
  };
  SDL_BlitSurface(surf_flaps, NULL, screen, &fP2);
  drawNum(statsObj->v2.vec.y, STATS_FLAP_OFFSET_X2 - 10, STATS_FLAP_OFFSET_Y);
}

void updateStats(GameObject *statsObj) {

}
