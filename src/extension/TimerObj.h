#ifndef TIMEROBJ_H
#define TIMEROBJ_H

#include <stdint.h>
#include <stdbool.h>
#include "Common.h"
#include "UtilObj.h"

void timerObjAddTime(GameObject *timerObj);
uint32_t timerObjElapsed(GameObject *timerObj);
void timerObjStart(GameObject *timerObj);
void timerObjStop(GameObject *timerObj);
void updateTimerAlarm(GameObject *timerObj);
void updateTimerRepeated(GameObject *timerObj);
void updateTimerConstant(GameObject *timerObj);
void initTimerObj(GameObject *timerObj, uint32_t duration, bool enable,
  void (*updateMode)(GameObject*), void (*endAction)(void));

#endif
