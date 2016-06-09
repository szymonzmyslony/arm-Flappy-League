#include "Control.h"

 bool go_left_first_player;
 bool go_right_first_player;
 bool go_right_second_player;
 bool go_left_second_player;
 
 bool pressed_left_first_player;
 bool pressed_right_first_player;
 bool pressed_right_second_player;
 bool pressed_left_second_player;
 



#define CHECK(name, pinNO, isPressed, goPlayer)     \        
  void name(void)                 \
  {                        \
   bool pin = digitalRead(pinNO)          \
   if (pin&&!isPressed){                  \
    isPressed = true;                     \
	goPlayer = true; }                     \
  else if(isPressed&&!pin){                \
	  isPressed = false; }	               \
}     

CHECK(check_left_first, LeftFirstPlayer, pressed_left_first_player, go_left_first_player)
CHECK(check_right_first, RightFirstPlayer, pressed_right_first_player, go_right_first_player)
CHECK(check_left_second, LeftSecondPlayer, pressed_left_second_player, go_left_second_player)
CHECK(check_right_second, RightSecondPlayer, pressed_right_second_player, go_right_second_player)








  







void moveLeft(GameObject *circObj) {
  circObj->v2.vec.x = -sideVelocity;
  circObj->v2.vec.y = -upVelocity;
}

void moveRight(GameObject *circObj) {
  circObj->v2.vec.x = +sideVelocity;
  circObj->v2.vec.y = -upVelocity;
}
