#include "TimerObj.h"

#define lastTime         timerObj->v1.u
#define elapsed          timerObj->v2.u
#define activationTime   timerObj->v3.u
#define enabled          timerObj->v4.b
#define runAction        timerObj->v5.func

inline void timerObjAddTime(GameObject *timerObj) {
  uint32_t now = SDL_GetTicks();
  elapsed += now - lastTime;
  lastTime = now;
}

uint32_t timerObjElapsed(GameObject *timerObj) {
  return elapsed;
}

void timerObjStart(GameObject *timerObj) {
  lastTime = SDL_GetTicks();
  enabled = true;
}

void timerObjStop(GameObject *timerObj) {
  enabled = false;
  timerObjAddTime(timerObj);
}

/** The timer runs the function once upon reaching the given duration
*/
void updateTimerAlarm(GameObject *timerObj) {
  if(enabled) {
    timerObjAddTime(timerObj);
    if(elapsed > activationTime) {
      runAction ();
      elapsed = 0;
      enabled = false;
    }
  }
}

/** The timer runs the function repeatedly upon reaching each multiple of the
* given duration
*/
void updateTimerRepeated(GameObject *timerObj) {
  if(enabled) {
    timerObjAddTime(timerObj);
    if(elapsed > activationTime) {
      runAction ();
      elapsed = 0;
    }
  }
}

/** The timer runs the function repeatedly until it reaches the given duration
*/
void updateTimerConstant(GameObject *timerObj) {
  if(enabled) {
    timerObjAddTime(timerObj);
    if(elapsed < activationTime) {
      runAction ();
    } else {
      enabled = false;
    }
  }
}

void initTimerObj(GameObject *timerObj, uint32_t duration, bool enable,
                 void (*updateMode)(GameObject*), void (*endAction)(void)) {
  lastTime = SDL_GetTicks();
  elapsed = 0;
  activationTime = duration;
  enabled = enable;
  runAction = endAction;
  timerObj->update = updateMode;
  timerObj->draw = NULL;
}
