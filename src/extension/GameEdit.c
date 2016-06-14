#include "GameEdit.h"

enum gameStates { MENU, MATCH, POSTMATCH };
static int gameState;
static bool soundEnabled;

//========================= Init Game States ===============================

void initMenu(void) {
  drawBackground();
  clearAllObjs();

  gameState = MENU;
  initMenuObj(gObjs[MAINMENU]);
  setSprite(gObjs[MAINMENU], surf_main_menu);

  initSquareObj(gObjs[TITLE], TITLE_OFFSET_X, TITLE_OFFSET_Y, TITLE_WIDTH,
    TITLE_HEIGHT, false);
  setSprite(gObjs[TITLE], surf_title);

  initPhysics();
  initSetup();
}

void initEnd(void) {
  drawBackground();

  gameState = POSTMATCH;
  initEndScreenObj(gObjs[ENDSCREEN]);
  setSprite(gObjs[ENDSCREEN], surf_end_menu);

  showStats(gObjs[STATS]);

  // Clear the Goal objects by overwriting them
  initTimerObj(gObjs[GOAL1], (0 * SECOND), true, &updateTimerAlarm,
    &playWhistleSound);
  initTimerObj(gObjs[GOAL2], (1 * SECOND), true, &updateTimerAlarm,
    &playWhistleSound);
  initTimerObj(gObjs[BALL], (2 * SECOND), true, &updateTimerAlarm,
    &playWhistleSound);
}

void initGame(void) {
  drawBackground();
  clearAllObjs();

  initPhysics();
  initSetup();
  initStatsObj(gObjs[STATS]);

  gameState = MATCH;
  // Setup score counters
  initSquareObj(gObjs[SCOREBOARD1], SCORE1_OFFSET_X, SCORE_OFFSET_Y,
    SCORE_WIDTH, SCORE_HEIGHT, false);
  gObjs[SCOREBOARD1]->v4.i = 0;
  gObjs[SCOREBOARD1]->draw = &drawScoreboard;
  setSprite(gObjs[SCOREBOARD1], surf_scoring);
  initSquareObj(gObjs[SCOREBOARD2], SCORE2_OFFSET_X, SCORE_OFFSET_Y,
    SCORE_WIDTH, SCORE_HEIGHT, false);
  gObjs[SCOREBOARD2]->v4.i = 0;
  gObjs[SCOREBOARD2]->draw = &drawScoreboard;
  setSprite(gObjs[SCOREBOARD2], surf_scoring);

  //Init Goals
  initSquareObj(gObjs[GOAL1], 0, GOAL_OFFSET_Y,
    GOAL_WIDTH, GOAL_HEIGHT, false);
  setSprite(gObjs[GOAL1], surf_goal);
  setCollFunc(gObjs[GOAL1], &scorePlayer2);

  initSquareObj(gObjs[GOAL2], WINDOW_WIDTH - GOAL_WIDTH, GOAL_OFFSET_Y,
    GOAL_WIDTH, GOAL_HEIGHT, false);
  setSprite(gObjs[GOAL2], surf_goal);
  setCollFunc(gObjs[GOAL2], &scorePlayer1);

  //Match Timer
  initTimerObj(gObjs[MATCH_TIMER], MATCH_LENGTH, true, &updateTimerAlarm,
    &initEnd);
  initSquareObj(gObjs[TIMEBOARD], MT_OFFSET_X, MT_OFFSET_Y,
    MT_WIDTH, MT_HEIGHT, true);
  setSprite(gObjs[TIMEBOARD], surf_timing);
  gObjs[TIMEBOARD]->draw = &drawTimer;
  gObjs[TIMEBOARD]->v3.g = gObjs[MATCH_TIMER];
}

/** Called when a new match has started. Initialises the ball and players.
*/
void initSetup(void) {
  //Init Players
  initCircleObj(gObjs[PLAYER1], PLAYER_SIZE / 2,
    PLAYER_OFFSET_X, PLAYER_OFFSET_Y, 0,  0);
  gObjs[PLAYER1]->v4.vec.x = 0;
  gObjs[PLAYER1]->v4.vec.y = 0;
  gObjs[PLAYER1]->draw = &drawAnimCircObj;
  setCollFunc(gObjs[PLAYER1], &collPlayer);
  gObjs[PLAYER1]->update = &updatePlayer;

  initCircleObj(gObjs[PLAYER2], PLAYER_SIZE / 2,
    screen->w - PLAYER_OFFSET_X, PLAYER_OFFSET_Y, 0, 0);
  gObjs[PLAYER2]->v4.vec.x = 0;
  gObjs[PLAYER2]->v4.vec.y = 0;
  gObjs[PLAYER2]->draw = &drawAnimCircObj;
  setCollFunc(gObjs[PLAYER2], &collPlayer);
  gObjs[PLAYER2]->update = &updatePlayer;

  // Randomly assign the player sprites
  setSprite(gObjs[PLAYER1], getRandomBirdSprite());
  do {
    setSprite(gObjs[PLAYER2], getRandomBirdSprite());
  } while(gObjs[PLAYER2]->sprite == gObjs[PLAYER1]->sprite);

  //Init Ball
  initCircleObj(gObjs[BALL], BALL_SIZE / 2,
    screen->w / 2, BALL_OFFSET_Y, 0, 0);
  setSprite(gObjs[BALL], surf_ball);
  setCollFunc(gObjs[BALL], &collBall);
  gObjs[BALL]->v4.vec.x = 0;
  gObjs[BALL]->v4.vec.y = 0;
  gObjs[BALL]->v5.f = 0;
  gObjs[BALL]->update = &updateBall;
  gObjs[BALL]->draw = &drawAnimCircObj;
}

/** Called after a goal is scored. Moves the ball and players to their
* start positions.
*/
void reinitSetup(void) {
  drawBackground();
  soundEnabled = true;

  gObjs[PLAYER1]->v1.vec.x = PLAYER_OFFSET_X;
  gObjs[PLAYER1]->v1.vec.y = PLAYER_OFFSET_Y;
  clearVar(&gObjs[PLAYER1]->v2);

  gObjs[PLAYER2]->v1.vec.x = screen->w - PLAYER_OFFSET_X;
  gObjs[PLAYER2]->v1.vec.y = PLAYER_OFFSET_Y;
  clearVar(&gObjs[PLAYER2]->v2);

  gObjs[BALL]->v1.vec.x = screen->w / 2;
  gObjs[BALL]->v1.vec.y = BALL_OFFSET_Y;
  gObjs[BALL]->v5.f = 0;
  clearVar(&gObjs[BALL]->v2);

  //Init Countdown
  initTimerObj(gObjs[WHISTLE_TIMER], 1 * SECOND, true, &updateTimerAlarm,
    &playWhistleSound);
}

void initPhysics(void) {
  initTimerObj(gObjs[GRAVITY_TIMER], UINT32_MAX, true, &updateTimerConstant,
    &applyAllGravity);
  initTimerObj(gObjs[AIR_RES_TIMER], UINT32_MAX, true, &updateTimerConstant,
    &applyAllAirResistance);
}

void drawBackground(void) {
  SDL_BlitSurface(surf_bg, NULL, screen, NULL);
}

//==================== GameObject Altering Functions =========================

void scorePlayer1(GameObject *colObj) {
  if(colObj == gObjs[BALL]) {
    gObjs[SCOREBOARD1]->v4.i++;
    reinitSetup();
    Mix_PlayChannel(-1, sound_goal, 0);
  }
}

void scorePlayer2(GameObject *colObj) {
  if(colObj == gObjs[BALL]) {
    gObjs[SCOREBOARD2]->v4.i++;
    reinitSetup();
    Mix_PlayChannel(-1, sound_goal, 0);
  }
}

void addAllVelocity(void) {
  for(int i = 0; i < MAX_OBJECTS; i++) {
    if(gObjs[i] != NULL && gObjs[i]->objType == COL_CIRCLE) {
      gObjs[i]->v2.vec.y += 5;
    }
  }
}

void applyAllGravity(void) {
  for(int i = 0; i < MAX_OBJECTS; i++) {
    if(gObjs[i] != NULL && gObjs[i]->objType == COL_CIRCLE) {
      gObjs[i]->v2.vec.y += 0.5;
    }
  }
}

void applyAllAirResistance(void) {
  for(int i = 0; i < MAX_OBJECTS; i++) {
    if(gObjs[i] != NULL && gObjs[i]->objType == COL_CIRCLE) {
      gObjs[i]->v2.vec.x *= 0.99;
    }
  }
}

void moveLeft(GameObject *circObj) {
  circObj->v2.vec.x = -sideVelocity;
  circObj->v2.vec.y = -upVelocity;
  circObj->v4.vec.x = 0;
  circObj->v4.vec.y = 1;
}

void moveRight(GameObject *circObj) {
  circObj->v2.vec.x = +sideVelocity;
  circObj->v2.vec.y = -upVelocity;
  circObj->v4.vec.x = 0;
  circObj->v4.vec.y = 0;
}

void drawScoreboard(GameObject *board){
  // (board->v4.i / 10) % 10, % 10 ensure the index stays bounded
  animate(board->sprite, (board->v4.i / 10 ) % 10, 0,
    SCORE_NWIDTH, board->v2.vec.y,
    board->v1.vec.x, board->v1.vec.y);

  animate(board->sprite, board->v4.i % 10, 0,
    SCORE_NWIDTH, board->v2.vec.y,
    board->v1.vec.x + SCORE_NWIDTH + SCORE_XGAP, board->v1.vec.y);
}

void drawTimer(GameObject *squareObj){
  int secondsLeft = (squareObj->v3.g->v3.u - squareObj->v3.g->v2.u) / SECOND;
  int mm = secondsLeft / 60;
  int ss = secondsLeft % 60;

  animate(squareObj->sprite, (mm / 10) % 10, 0,
    MT_NWIDTH, MT_HEIGHT,
    squareObj->v1.vec.x, MT_OFFSET_Y);

  animate(squareObj->sprite, mm % 10, 0,
    MT_NWIDTH, MT_HEIGHT,
    squareObj->v1.vec.x + MT_NWIDTH + MT_XGAP, MT_OFFSET_Y);

  animate(squareObj->sprite, 10, 0,
    MT_NWIDTH, MT_HEIGHT,
    squareObj->v1.vec.x + 2 * (MT_NWIDTH + MT_XGAP), MT_OFFSET_Y);

  animate(squareObj->sprite, ss / 10, 0,
    MT_NWIDTH, MT_HEIGHT,
    squareObj->v1.vec.x + 3 * (MT_NWIDTH + MT_XGAP), MT_OFFSET_Y);

  animate(squareObj->sprite, ss % 10, 0,
    MT_NWIDTH, MT_HEIGHT,
    squareObj->v1.vec.x + 4 * (MT_NWIDTH + MT_XGAP), MT_OFFSET_Y);
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
        gObjs[STATS]->flapsP1++;
        buttonDownP1Left = false;
      }

      if(buttonDownP1Right) {
        moveRight(gObjs[PLAYER1]);
        gObjs[STATS]->flapsP1++;
        buttonDownP1Right = false;
      }

      if(buttonDownP2Left) {
        moveLeft(gObjs[PLAYER2]);
        gObjs[STATS]->flapsP2++;
        buttonDownP2Left = false;
      }

      if(buttonDownP2Right) {
        moveRight(gObjs[PLAYER2]);
        gObjs[STATS]->flapsP2++;
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
  if(soundEnabled) {
    Mix_Volume(-1, MIX_MAX_VOLUME);
    Mix_VolumeMusic(MIX_MAX_VOLUME);
  } else {
    Mix_Volume(-1, 0);
    Mix_VolumeMusic(0);
  }
  soundEnabled = !soundEnabled;
}

bool getSoundState(void) {
  return soundEnabled;
}
