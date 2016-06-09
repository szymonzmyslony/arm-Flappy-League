#include "Control.h"

#define CHECK(name, pinNO, isPressed, goPlayer)     \
void name(void) {                                   \
  bool pin = digitalRead(pinNO);                    \
  if(pin && !isPressed) {                           \
    isPressed = true;                               \
	  goPlayer = true;                                \
  } else if(isPressed && !pin) {                    \
	  isPressed = false;                              \
  }	                                                \
}

static bool buttonP1Left;
static bool buttonP1Right;
static bool buttonP2Left;
static bool buttonP2Right;

CHECK(updateButtonDownP1Left, LeftFirstPlayer, buttonP1Left,
      buttonDownP1Left)
CHECK(updateButtonDownP1Right, RightFirstPlayer, buttonP1Right,
      buttonDownP1Right)
CHECK(updateButtonDownP2Left, LeftSecondPlayer, buttonP2Left,
      buttonDownP2Left)
CHECK(updateButtonDownP2Right, RightSecondPlayer, buttonP2Right,
      buttonDownP2Right)

void updateButtonsStatus(void) {
  updateButtonDownP1Left();
  updateButtonDownP1Right();
  updateButtonDownP2Left();
  updateButtonDownP2Right();
}
