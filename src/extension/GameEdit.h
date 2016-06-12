#ifndef GAMEEDIT_H
#define GAMEEDIT_H
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "Control.h"
#include "Common.h"
#include "CircleObj.h"
#include "SquareObj.h"
#include "TimerObj.h"
#include "UtilObj.h"
#include "Menu.h"

#define upVelocity   10.0
#define sideVelocity  5.0

#define MATCH_LENGTH 120 * SECOND

enum sizesAndPositions {
  PLAYER_SIZE = 64,
  BALL_SIZE = 64,
  SCORE_WIDTH = 50,
  SCORE_HEIGHT = 90,
  SCORE_OFFSET_Y = WINDOW_HEIGHT / 4,
  SCORE1_OFFSET_X = WINDOW_WIDTH / 2 + 100 - SCORE_WIDTH,
  SCORE2_OFFSET_X = WINDOW_WIDTH / 2 - 100,
  GOAL_OFFSET = WINDOW_HEIGHT / 4,
  GOAL_WIDTH = WINDOW_WIDTH / 9,
  GOAL_HEIGHT = WINDOW_HEIGHT / 3
};

void moveLeft(GameObject *circObj);
void moveRight(GameObject *circObj);

void initMenu(void);
void initEnd(void);
void initGame(void);
void initSetup(void);

void addAllVelocity(void);
void applyAllGravity(void);
void applyAllAirResistance(void);

void scorePlayer1(GameObject *colObj);
void scorePlayer2(GameObject *colObj);
void drawScoreboard(GameObject *board);
void toggleSound(void);
bool getSoundState(void);

#endif
