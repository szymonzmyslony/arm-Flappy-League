#ifndef CONTROL_H
#define CONTROL_H
#include "Common.h"
#include "Main.h"

/** GPIO pins
*/
enum Pins {
  RightSecondPlayer = 12,
  RightFirstPlayer = 1,
  LeftFirstPlayer = 4,
  LeftSecondPlayer = 13
};

void updateButtonsStatus(void);
void updateButtonDownP1Left(void);
void updateButtonDownP1Right(void);
void updateButtonDownP2Left(void);
void updateButtonDownP2Right(void);

void handleButtonStatus(void);

#endif
