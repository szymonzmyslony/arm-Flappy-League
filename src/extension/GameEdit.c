#include "GameEdit.h"

void initMenu(void) {
  initSetup();
}

void initGame(void) {
  // Setup score counters
  gObjs[SCOREBOARD1] = initSquareObj(SCORE1_OFFSET_X, SCORE_OFFSET_Y,
                                      SCORE_WIDTH, SCORE_HEIGHT, false);
  gObjs[SCOREBOARD1]->v4.i = 0;
  gObjs[SCOREBOARD2] = initSquareObj(SCORE2_OFFSET_X, SCORE_OFFSET_Y,
                                      SCORE_WIDTH, SCORE_HEIGHT, false);
  gObjs[SCOREBOARD2]->v4.i = 0;

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

  //Init Goals
  gObjs[GOAL1] = initSquareObj(0, GOAL_OFFSET,
                           GOAL_WIDTH, GOAL_HEIGHT, false);
  setCollFunc(gObjs[GOAL1], &scorePlayer1);

  gObjs[GOAL2] = initSquareObj(WINDOW_WIDTH - GOAL_WIDTH, GOAL_OFFSET,
                           GOAL_WIDTH, GOAL_HEIGHT, false);
  setCollFunc(gObjs[GOAL2], &scorePlayer2);

  //Init Physics
  gObjs[9] = initTimerObj(UINT32_MAX, true, &updateTimerConstant,
                          &applyAllGravity);
  gObjs[10] = initTimerObj(UINT32_MAX, true, &updateTimerConstant,
                          &applyAllAirResistance);

  //Init Countdown
  gObjs[11] = initTimerObj(3000, true, &updateTimerAlarm,
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
