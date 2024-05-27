#pragma once

typedef struct
{
    float accumulator;
    float prevTime;
    float deltaTime;
} Time;

void init_time(Time *t);
void update_time(Time *t, float currTime);
