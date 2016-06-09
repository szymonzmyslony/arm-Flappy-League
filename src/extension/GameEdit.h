#ifndef GAMEEDIT_H
#define GAMEEDIT_H
#include <stdint.h>
#include <stdbool.h>
#include "Control.h"
#include "Structs.h"
#include "CircleObj.h"
#include "SquareObj.h"
#include "UtilObj.h"

#define upVelocity   10.0
#define sideVelocity  5.0

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
void initGame(void);
void initSetup(void);

void scorePlayer1(GameObject *colObj);
void scorePlayer2(GameObject *colObj);

#endif
