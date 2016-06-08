#include "UtilObj.h"
//============================ Timer Object =============================
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

GameObject *initTimerObj(uint32_t duration, bool enable,
                 void (*updateMode)(GameObject*), void (*endAction)(void)) {
  GameObject *timerObj = (GameObject*)calloc(1, sizeof(GameObject));
  lastTime = SDL_GetTicks();
  elapsed = 0;
  activationTime = duration;
  enabled = enable;
  runAction = endAction;
  timerObj->update = updateMode;

  return timerObj;
}

//=============================== Timer Actions ===========================
void addAllVelocity() {
  for(int i = 0; i < MAX_OBJECTS; i++) {
    if(gObjs[i] != NULL && gObjs[i]->colliderType == COL_CIRCLE) {
      gObjs[i]->v2.vec.y += 5;
    }
  }
}

//=============================== Input Objects ===========================

//============================= Setter Functions ==========================
void setSprite(GameObject *gameObj, SDL_Surface *newSprite) {
  gameObj->sprite = newSprite;
}
