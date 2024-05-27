#include "time.h"

void init_time(Time *t)
{
    t->accumulator = t->prevTime = t->deltaTime = 0;
}

void update_time(Time *t, float currTime)
{

    t->deltaTime = currTime - t->prevTime;
    t->prevTime = currTime;

    t->accumulator += t->deltaTime;
}