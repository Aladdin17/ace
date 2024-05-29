#pragma once
#include <ace/math/vec3.h>
#include <stdbool.h>
#include <ace/physics/phys_world.h>
#include "timer.h"

typedef struct orbit_camera
{
    float radius;
    float pitch_angle;
    float yaw_angle;
    ac_vec3 target;
} orbit_camera;

typedef struct cue_stick
{
    unsigned target;
    float pitch;
    float yaw;
    float power;
    bool strike;
    bool visible;
    void (*draw)(const struct cue_stick*, const ac_vec3* position, float radius);
} cue_stick;

typedef struct pool_ball
{
    unsigned physics_id;
    ac_vec3 color;
    float radius;
    char tag[16];
    void (*draw)(const struct pool_ball*, const ac_vec3* position);
} pool_ball;

typedef struct pool_table
{
    unsigned physics_ids[6];           // physics id for lookup of properties
    void (*draw)(const struct pool_table*);  // function pointer to draw the table
} pool_table;

typedef struct pool_app
{
    frame_time timer;          // stores frame time information
    orbit_camera main_camera;  // used to orbit the camera around the table
    cue_stick cue_stick;       // cue cue_stick
    pool_ball balls[11];       // 0 is cue ball, plus 10 targets
    int num_balls;             // number of balls
    pool_table table;          // pool table
    PhysWorld physics_world;   // physics world
    bool show_minimap;         // show the minimap
} pool_app;
