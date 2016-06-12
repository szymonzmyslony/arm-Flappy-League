#include "GameEdit.h"

enum gameStates { MENU, MATCH, POSTMATCH };
static int gameState;
static bool soundEnabled;

//========================= Init Game States ===============================

void initMenu(void) {
  gameState = MENU;
  initMenuObj(gObjs[MAINMENU]);
  setSprite(gObjs[MAINMENU], main_menu_sprite);
  initSetup();
}

void initEnd(void) {
  gameState = POSTMATCH;
  // Clear the Goal and ball objects
  initTimerObj(gObjs[GOAL1], (0 * SECOND), true, &updateTimerAlarm,
                          &playWhistleSound);
  initTimerObj(gObjs[GOAL2], (1 * SECOND), true, &updateTimerAlarm,
                          &playWhistleSound);
  initTimerObj(gObjs[BALL], (2 * SECOND), true, &updateTimerAlarm,
                          &playWhistleSound);
  initMenuObj(gObjs[ENDSCREEN]);
}

void initGame(void) {
  gameState = MATCH;
  // Setup score counters
  initSquareObj(gObjs[SCOREBOARD1], SCORE1_OFFSET_X, SCORE_OFFSET_Y,
                                      SCORE_WIDTH, SCORE_HEIGHT, false);
  gObjs[SCOREBOARD1]->v4.i = 0;
  gObjs[SCOREBOARD1]->draw = &drawScoreboard;
  gObjs[SCOREBOARD1]->sprite = surf_scoring;
  initSquareObj(gObjs[SCOREBOARD2], SCORE2_OFFSET_X, SCORE_OFFSET_Y,
                                      SCORE_WIDTH, SCORE_HEIGHT, false);
  gObjs[SCOREBOARD2]->v4.i = 0;
  gObjs[SCOREBOARD2]->draw = &drawScoreboard;
  gObjs[SCOREBOARD2]->sprite = surf_scoring;

  //Init Goals
  initSquareObj(gObjs[GOAL1], 0, GOAL_OFFSET,
    GOAL_WIDTH, GOAL_HEIGHT, false);
  setSprite(gObjs[GOAL1], surf_goal);
  setCollFunc(gObjs[GOAL1], &scorePlayer1);

  initSquareObj(gObjs[GOAL2], WINDOW_WIDTH - GOAL_WIDTH, GOAL_OFFSET,
    GOAL_WIDTH, GOAL_HEIGHT, false);
  setSprite(gObjs[GOAL2], surf_goal);
  setCollFunc(gObjs[GOAL2], &scorePlayer2);

  //Init Physics
  initTimerObj(gObjs[GRAVITY_TIMER], UINT32_MAX, true, &updateTimerConstant,
    &applyAllGravity);
  initTimerObj(gObjs[AIR_RES_TIMER], UINT32_MAX, true, &updateTimerConstant,
    &applyAllAirResistance);

  //Match Timer
  initTimerObj(gObjs[MATCH_TIMER], MATCH_LENGTH, true, &updateTimerAlarm,
    &initEnd);

  initSetup();
}

void initSetup(void) {
  //Init Players
  initCircleObj(gObjs[PLAYER1], PLAYER_SIZE / 2, 100, 300, 0,  0);
  setSprite(gObjs[PLAYER1], surf_bird1);

  initCircleObj(gObjs[PLAYER2], PLAYER_SIZE / 2, screen->w - 100, 300, 0,  0);
  setSprite(gObjs[PLAYER2], surf_bird2);

  //Init Ball
  initCircleObj(gObjs[BALL], BALL_SIZE / 2, screen->w/2, 300, 0, 0);
  setSprite(gObjs[BALL], surf_ball);
  setCollFunc(gObjs[BALL], &playCollKickSound);

  //Init Countdown
  initTimerObj(gObjs[WHISTLE_TIMER], 1 * SECOND, true, &updateTimerAlarm,
                          &playWhistleSound);
}

//==================== GameObject Altering Functions =========================

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

void addAllVelocity(void) {
  for(int i = 0; i < MAX_OBJECTS; i++) {
    if(gObjs[i] != NULL && gObjs[i]->colliderType == COL_CIRCLE) {
      gObjs[i]->v2.vec.y += 5;
    }
  }
}

void applyAllGravity(void) {
  for(int i = 0; i < MAX_OBJECTS; i++) {
    if(gObjs[i] != NULL && gObjs[i]->colliderType == COL_CIRCLE) {
      gObjs[i]->v2.vec.y += 0.5;
    }
  }
}

void applyAllAirResistance(void) {
  for(int i = 0; i < MAX_OBJECTS; i++) {
    if(gObjs[i] != NULL && gObjs[i]->colliderType == COL_CIRCLE) {
      gObjs[i]->v2.vec.x *= 0.99;
    }
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

void drawScoreboard(GameObject *board){
  animate(board->sprite, board->v4.i%10, 0, 62.5, 73, board->v1.vec.x, 
          board->v1.vec.y);
}

//=========================== IO Effectors ==================================

void handleButtonStatus(void) {
  switch(gameState) {
    case MENU:
      if (buttonDownP1Left) {
        incrementMenu(gObjs[MAINMENU]);
        buttonDownP1Left = false;
      }

      if (buttonDownP1Right) {
        selectMenu(gObjs[MAINMENU]);
        buttonDownP1Right = false;
      }
      if(buttonDownP2Left) {
        buttonDownP2Left = false;
      }

      if(buttonDownP2Right) {
        buttonDownP2Right = false;
      }
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
      if (buttonDownP1Left) {
        incrementMenu(gObjs[ENDSCREEN]);
        buttonDownP1Left = false;
      }
      if (buttonDownP1Right) {
        selectEndMenu(gObjs[ENDSCREEN]);
        buttonDownP1Right = false;
      }
      if(buttonDownP2Left) {
        buttonDownP2Left = false;
      }

      if(buttonDownP2Right) {
        buttonDownP2Right = false;
      }
      break;
  }
}

void toggleSound(void) {
  soundEnabled = !soundEnabled;
}
