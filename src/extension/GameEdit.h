#ifndef GAMEEDIT_H
#define GAMEEDIT_H
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "Control.h"
#include "Common.h"
#include "CircleObj.h"
#include "SquareObj.h"
#include "TimerObj.h"
#include "UtilObj.h"
#include "Stats.h"
#include "Menu.h"
#include "GameChangers.h"

#define upVelocity   10.0
#define sideVelocity  5.0

#define MATCH_LENGTH 120 * SECOND

enum sizesAndPositions {
  PLAYER_SIZE = 64,
  BALL_SIZE = 64,
  MT_XGAP = 4,
  MT_NWIDTH = 14,
  MT_WIDTH = 5 * MT_NWIDTH + 4 * MT_XGAP,
  MT_HEIGHT = 26,
  MT_OFFSET_X = WINDOW_WIDTH / 2 - MT_WIDTH / 2,
  MT_OFFSET_Y = 122,
  SCORE_XGAP = 10,
  SCORE_NWIDTH = 41,
  SCORE_WIDTH = 2 * SCORE_NWIDTH + SCORE_XGAP,
  SCORE_HEIGHT = 80,
  SCORE_OFFSET_Y = MT_OFFSET_Y + MT_HEIGHT + 6,
  SCORE_SCORE_GAP = 24,
  SCORE1_OFFSET_X = WINDOW_WIDTH / 2 - SCORE_SCORE_GAP - SCORE_WIDTH,
  SCORE2_OFFSET_X = WINDOW_WIDTH / 2 + SCORE_SCORE_GAP,
  TITLE_WIDTH = 543,
  TITLE_HEIGHT = 90,
  TITLE_OFFSET_X = (WINDOW_WIDTH / 2) - (TITLE_WIDTH / 2),
  TITLE_OFFSET_Y = 30,
  GOAL_WIDTH = WINDOW_WIDTH / 9,
  GOAL_HEIGHT = WINDOW_HEIGHT / 3
};

void moveLeft(GameObject *circObj);
void moveRight(GameObject *circObj);

void initMenu(void);
void initEnd(void);
void initGame(void);
void initSetup(void);
void initPhysics(void);
void drawBackground(void);

void addAllVelocity(void);
void applyAllGravity(void);
void applyAllAirResistance(void);

void scorePlayer1(GameObject *colObj);
void scorePlayer2(GameObject *colObj);
void drawScoreboard(GameObject *board);
void drawTimer(GameObject *matchTimer);
void toggleSound(void);
bool getSoundState(void);

#endif
