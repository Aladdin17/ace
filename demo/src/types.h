#pragma once
#include <ace/math/vec3.h>
#include <stdbool.h>
#include <ace/physics/phys_world.h>
#include "timer.h"

typedef struct orbit_camera
{
    float   radius;
    float   min_radius;
    float   max_radius;
    float   pitch_angle;
    float   min_pitch_angle;
    float   max_pitch_angle;
    float   yaw_angle;
    float   rotation_step;
    float   zoom_step;
    ac_vec3 target;
} orbit_camera;

typedef struct cue_stick
{
    // properties
    int   target_ball;  // this must be < num_balls
    float pitch_angle;
    float min_pitch_angle;
    float max_pitch_angle;
    float yaw;
    float power;
    float power_step;
    float max_power_ms;
    float rotation_step;

    // runtime
    bool strike;

    // rendering
    ac_vec3 color;
    float   length;
    float   radius;
    float   draw_distance;  // in m from target ball
    bool    visible;
    void (*draw)(const struct cue_stick*, const ac_vec3* target_position, float target_radius);
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
    unsigned pocket_physics_ids[4];
    ac_vec3 leg_centers[4];
    ac_vec3 cushion_centers[4];

    void (*draw)(const struct pool_table*, bool orthograhic);
} pool_table;

enum ball_layout
{
    ball_layout_triangle,
    ball_layout_rectangle
};
typedef struct pool_app
{
    // simulation properties
    frame_time timer;          ///< stores frame time information
    orbit_camera main_camera;  ///< used to orbit the camera around the table
    cue_stick cue_stick;       ///< cue cue_stick
    PhysWorld physics_world;   ///< physics world

    // pool balls
    pool_ball* balls;          ///* 0 is cue ball
    int num_balls;             ///* number of balls
    int target_entity_info;    // entity to show
    // pool table
    pool_table table;          ///< pool table
    int ball_layout;           ///< 0 = 9-ball, 1 = 8-ball

    // misc
    bool show_minimap;         ///< show the minimap
    bool show_entity_info;     ///< show info panel
} pool_app;
