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
    unsigned physics_ids[5];
    float length;
    float width;
    float top_depth;
    float cushion_height;
    float cushion_width;
    float pocket_radius;

    float leg_length;

    // relative to the origin of the table
    // which is the center of the playing surface
    ac_vec3 pocket_centers[4];
    ac_vec3 leg_centers[4];
    ac_vec3 cushion_centers[4];

    void (*draw)(const struct pool_table*);
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
    bool show_entity_info;     // show info panel
    int target_entity_info;    // entity to show
} pool_app;
