#ifndef GAMECHANGERS_H
#define GAMECHANGERS_H

/** All easy-to-modify variables are stored here. These directly impact
* gameplay
*/
// Minimum rebounce velocity
#define MINBOUNCE 15.0

enum positions {
  PLAYER_OFFSET_X = 100,
  PLAYER_OFFSET_Y = 300,
  BALL_OFFSET_Y = 300,
  FLOOR_Y = WINDOW_HEIGHT - 30,
  GOAL_FLOOR_DY = 405,
  GOAL_OFFSET_Y = FLOOR_Y - GOAL_FLOOR_DY
};

#endif
