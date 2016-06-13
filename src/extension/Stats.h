#ifndef STATS_H
#define STATS_H
#include "Common.h"
#include "UtilObj.h"

#define kicksP1 v1.vec.x
#define kicksP2 v1.vec.y
#define flapsP1 v2.vec.x
#define flapsP2 v2.vec.y

void initStatsObj(GameObject *statsObj);
void showStats(GameObject *statsObj);
void drawStats(GameObject *statsObj);
void updateStats(GameObject *statsObj);

#endif
