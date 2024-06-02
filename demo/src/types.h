/**
 * \file
 * \author Christien Alden
 * \brief Defines the types needed by the application
 */
#pragma once
#include "timer.h"
#include <ace/math/vec2.h>
#include <ace/math/vec3.h>
#include <ace/physics/phys_world.h>
#include <stdbool.h>

/**
 * \struct orbit_camera
 * \brief Structure to hold the data for an orbit camera.
 */
typedef struct orbit_camera
{
    float   radius;           ///< distance from the target
    float   min_radius;       ///< minimum distance from the target
    float   max_radius;       ///< maximum distance from the target
    float   pitch_angle;      ///< pitch angle of the camera
    float   min_pitch_angle;  ///< minimum pitch angle of the camera
    float   max_pitch_angle;  ///< maximum pitch angle of the camera
    float   yaw_angle;        ///< yaw angle of the camera
    float   rotation_step;    ///< step size for rotation
    float   zoom_step;        ///< step size for zooming
    ac_vec3 target;           ///< target of the camera
} orbit_camera;

/**
 * \struct cue_stick
 * \brief Structure to hold the data for the cue stick.
 */
typedef struct cue_stick
{
    // properties
    int   target_ball;        ///< index of the target ball, must be less than num_balls
    float pitch_angle;        ///< angle of the stick
    float min_pitch_angle;    ///< minimum angle of the stick
    float max_pitch_angle;    ///< maximum angle of the stick
    float yaw;                ///< yaw angle of the stick
    float power;              ///< power of the stick, [0, 1]
    float power_step;         // step size for power, [0, 1]
    float max_power_newtons;  ///< maximum power in newtons
    float rotation_step;      ///< step size for rotation

    // runtime
    bool strike;  ///< true if the stick is striking the cue ball

    // rendering
    ac_vec3 color;          ///< color of the stick
    float   length;         ///< length of the stick
    float   radius;         ///< radius of the stick
    float   draw_distance;  ///< distance to draw the stick
    bool    visible;        ///< true if the stick is visible
    /// Function to draw the cue stick
    void (*draw)(const struct cue_stick*, const ac_vec3* target_position, float target_radius);
} cue_stick;

/**
 * \struct pool_ball
 * \brief Structure to hold the data for a pool ball.
 */
typedef struct pool_ball
{
    unsigned physics_id;  ///< physics id
    ac_vec3  color;       ///< color of the ball
    float    radius;      ///< radius of the ball
    /// Function to draw the pool ball
    void (*draw)(const struct pool_ball*, const ac_vec3* position);
} pool_ball;

/**
 * \struct pool_table
 * \brief Structure to hold the data for a pool table.
 */
typedef struct pool_table
{
    // properties
    ac_vec3 surface_center;  ///< center of the playing surface in world coords

    // physics
    unsigned physics_ids[5];         ///< physics ids for the table, cushions, and legs
    ac_vec3  pocket_centers[4];      ///< centers of the pockets
    unsigned pocket_physics_ids[4];  ///< physics ids for the pockets
    ac_vec3  leg_centers[4];         ///< centers of the legs
    ac_vec3  cushion_centers[4];     ///< centers of the cushions

    // rendering
    float   length;          ///< length of the table
    float   width;           ///< width of the table
    float   top_depth;       ///< depth of the top of the table
    float   cushion_height;  ///< height of the cushions
    float   cushion_width;   ///< width of the cushions
    float   pocket_radius;   ///< radius of the pockets
    float   leg_length;      ///< length of the legs
    ac_vec3 surface_color;   ///< color of the playing surface
    ac_vec3 cushion_color;   ///< color of the cushions
    ac_vec3 leg_color;       ///< color of the legs
    /// Function to draw the pool table
    void (*draw)(const struct pool_table*, bool orthograhic);
} pool_table;

/**
 * \enum ball_layout
 * \brief Enum to define the layout of the balls on the table.
 */
enum ball_layout
{
    ball_layout_triangle,
    ball_layout_rectangle
};

/**
 * \struct pool_app
 * \brief Structure to hold the data for the pool application.
 */
typedef struct pool_app
{
    // simulation properties
    frame_time   timer;          ///< stores frame time information
    orbit_camera main_camera;    ///< used to orbit the camera around the table
    cue_stick    cue_stick;      ///< cue cue_stick
    PhysWorld    physics_world;  ///< physics world

    // pool balls
    pool_ball* balls;                  ///< 0 is cue ball
    int        num_balls;              ///< number of balls
    ac_vec2    cue_start_position;     ///< position of the cue ball on the table 0-1 (x, z)
    ac_vec2    target_start_position;  ///< position of the target ball on the table 0-1 (x, z)
    float      ball_drop_height;       ///< height to drop the balls from

    // pool table
    pool_table table;              ///< pool table
    int        ball_layout;        ///< based on ball_layout enum
    float      surface_roughness;  ///< surface roughness

    // misc
    bool  show_minimap;      ///< show the minimap
    bool  show_entity_info;  ///< show info panel
    bool  show_controls;     ///< show controls overlay
    float min_ball_speed;    ///< minimum speed for a ball to be considered moving
    float y_threshold;       ///< y position threshold for a ball to be considered off the table
} pool_app;
