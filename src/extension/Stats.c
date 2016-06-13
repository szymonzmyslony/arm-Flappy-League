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
  
}

void updateStats(GameObject *statsObj) {

}
