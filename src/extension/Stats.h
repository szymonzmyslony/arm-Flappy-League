#ifndef STATS_H
#define STATS_H
#include "Common.h"
#include "UtilObj.h"

#define kicksP1 v1.vec.x
#define kicksP2 v1.vec.y
#define flapsP1 v2.vec.x
#define flapsP2 v2.vec.y

enum statsSizesAndPositions {
  STATS_ENTRY_GAP = 24,

  STATS_KICK_OFFSET_X1 = WINDOW_WIDTH * 1 / 4 - NUM_3WIDTH / 2 - 100,
  STATS_FLAP_OFFSET_X1 = STATS_KICK_OFFSET_X1,

  STATS_KICK_OFFSET_X2 = WINDOW_WIDTH * 3 / 4 - NUM_3WIDTH / 2,
  STATS_FLAP_OFFSET_X2 = STATS_KICK_OFFSET_X2,

  STATS_KICK_OFFSET_Y = WINDOW_HEIGHT / 2,
  STATS_FLAP_OFFSET_Y = STATS_KICK_OFFSET_Y + NUM_HEIGHT + STATS_ENTRY_GAP,
};

void initStatsObj(GameObject *statsObj);
void showStats(GameObject *statsObj);
void drawStats(GameObject *statsObj);
void updateStats(GameObject *statsObj);

#endif
