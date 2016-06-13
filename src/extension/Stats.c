#include "Stats.h"

#define stat_kick_p1 gObjs[STATS]->v1.vec.x
#define stat_kick_p2 gObjs[STATS]->v1.vec.y
#define stat_flap_p1 gObjs[STATS]->v2.vec.x
#define stat_flap_p2 gObjs[STATS]->v2.vec.y

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
