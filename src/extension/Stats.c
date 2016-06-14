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
  // Draw Number of kicks for P1
  drawNum(statsObj->v1.vec.x, STATS_KICK_OFFSET_X1, STATS_KICK_OFFSET_Y);
  drawNum(statsObj->v2.vec.x, STATS_FLAP_OFFSET_X1, STATS_FLAP_OFFSET_Y);

  drawNum(statsObj->v1.vec.y, STATS_FLAP_OFFSET_X2, STATS_KICK_OFFSET_Y);
  drawNum(statsObj->v2.vec.y, STATS_FLAP_OFFSET_X2, STATS_FLAP_OFFSET_Y);
}

void updateStats(GameObject *statsObj) {

}
