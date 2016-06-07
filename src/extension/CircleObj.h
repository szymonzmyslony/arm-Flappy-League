#ifndef CIRCLE_OBJ_H
#define CIRCLE_OBJ_H

GameObject *initCircleObj(float r, float x, float y, float vx, float vy);
void drawCircleObj(GameObject *circObj);
void updateCircleObj(GameObject *circObj);
void drawCentredObj(GameObject *gameObj);

#endif
