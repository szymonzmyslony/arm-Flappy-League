#include "GameEdit.h"

enum gameStates { MENU, MATCH, POSTMATCH };
static int gameState;

void initMenu(void) {
  gameState = MENU;
  initSetup();
}

void initEnd(void) {
  gameState = POSTMATCH;
  gObjs[GOAL1] = initTimerObj(0 * SECOND, true, &updateTimerAlarm,
                          &playWhistleSound);
  gObjs[GOAL2] = initTimerObj((1 * SECOND), true, &updateTimerAlarm,
                          &playWhistleSound);
  gObjs[BALL] = initTimerObj(2 * SECOND, true, &updateTimerAlarm,
                          &playWhistleSound);
}

void initGame(void) {
  gameState = MATCH;
  // Setup score counters
  gObjs[SCOREBOARD1] = initSquareObj(SCORE1_OFFSET_X, SCORE_OFFSET_Y,
                                      SCORE_WIDTH, SCORE_HEIGHT, false);
  gObjs[SCOREBOARD1]->v4.i = 0;
  gObjs[SCOREBOARD2] = initSquareObj(SCORE2_OFFSET_X, SCORE_OFFSET_Y,
                                      SCORE_WIDTH, SCORE_HEIGHT, false);
  gObjs[SCOREBOARD2]->v4.i = 0;

  //Init Goals
  gObjs[GOAL1] = initSquareObj(0, GOAL_OFFSET,
    GOAL_WIDTH, GOAL_HEIGHT, false);
  setSprite(gObjs[GOAL1], surf_goal);
  setCollFunc(gObjs[GOAL1], &scorePlayer1);

  gObjs[GOAL2] = initSquareObj(WINDOW_WIDTH - GOAL_WIDTH, GOAL_OFFSET,
    GOAL_WIDTH, GOAL_HEIGHT, false);
  setSprite(gObjs[GOAL2], surf_goal);
  setCollFunc(gObjs[GOAL2], &scorePlayer2);

  //Init Physics
  gObjs[9] = initTimerObj(UINT32_MAX, true, &updateTimerConstant,
    &applyAllGravity);
  gObjs[10] = initTimerObj(UINT32_MAX, true, &updateTimerConstant,
    &applyAllAirResistance);

  //Match Timer
  gObjs[12] = initTimerObj(MATCH_TIMER, true, &updateTimerAlarm,
    &initEnd);

  initSetup();
}

void initSetup(void) {
  //Init Players
  gObjs[PLAYER1] = initCircleObj(PLAYER_SIZE / 2, 100, 300, 0,  0);
  setSprite(gObjs[PLAYER1], surf_bird1);

  gObjs[PLAYER2] = initCircleObj(PLAYER_SIZE / 2, screen->w - 100, 300, 0,  0);
  setSprite(gObjs[PLAYER2], surf_bird2);

  //Init Ball
  gObjs[BALL] = initCircleObj(BALL_SIZE / 2, screen->w/2, 300, 0, 0);
  setSprite(gObjs[BALL], surf_ball);

  //Init Countdown
  gObjs[11] = initTimerObj(1 * SECOND, true, &updateTimerAlarm,
                          &playWhistleSound);
}

void scorePlayer1(GameObject *colObj) {
  if(colObj == gObjs[BALL]) {
    gObjs[SCOREBOARD1]->v4.i++;
    initSetup();
    Mix_PlayChannel(-1, sound_goal, 0);
  }
}

void scorePlayer2(GameObject *colObj) {
  if(colObj == gObjs[BALL]) {
    gObjs[SCOREBOARD2]->v4.i++;
    initSetup();
    Mix_PlayChannel(-1, sound_goal, 0);
  }
}

void moveLeft(GameObject *circObj) {
  circObj->v2.vec.x = -sideVelocity;
  circObj->v2.vec.y = -upVelocity;
}

void moveRight(GameObject *circObj) {
  circObj->v2.vec.x = +sideVelocity;
  circObj->v2.vec.y = -upVelocity;
}

void handleButtonStatus(void) {
  switch(gameState) {
    case MENU:
      break;

    case MATCH:
      if(buttonDownP1Left) {
        moveLeft(gObjs[PLAYER1]);
        buttonDownP1Left = false;
      }

      if(buttonDownP1Right) {
        moveRight(gObjs[PLAYER1]);
        buttonDownP1Right = false;
      }

      if(buttonDownP2Left) {
        moveLeft(gObjs[PLAYER2]);
        buttonDownP2Left = false;
      }

      if(buttonDownP2Right) {
        moveRight(gObjs[PLAYER2]);
        buttonDownP2Right = false;
      }
      break;

    case POSTMATCH:
      break;
  }
}
