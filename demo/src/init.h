/**
 * \file
 * \author Christien Alden
 * \brief Initialisation functions for the pool app.
 */
#pragma once

//---------------------------------------------------------------------
// Opaque Types
//---------------------------------------------------------------------

typedef struct pool_app     pool_app;
typedef struct PhysWorld    PhysWorld;
typedef struct pool_ball    pool_ball;
typedef struct pool_table   pool_table;
typedef struct cue_stick    cue_stick;
typedef struct orbit_camera orbit_camera;
typedef struct frame_time   frame_time;
typedef union ac_vec2       ac_vec2;
typedef union ac_vec3       ac_vec3;

/**
 * \brief Initialises the misc properties of the app.
 * \param[out] state The app to initialise.
 */
void initialise_misc(pool_app* state);
/**
 * \brief Initialises the orbit camera.
 * \param[out] camera The camera to initialise.
 */
void initialise_orbit_camera(orbit_camera* camera);
/**
 * \brief Initialises the frame time.
 * \param[out] time The frame time to initialise.
 */
void initialise_frame_time(frame_time* time);
/**
 * \brief Initialises the physics world.
 * \param[out] world The physics world to initialise.
 * \param[in] update_rate The rate at which the physics world should update.
 */
void initialise_physics_world(PhysWorld* world, int update_rate);
/**
 * \brief Initialises the pool balls.
 * \param[out] world The physics world to add the balls to.
 * \param[out] balls_ptr The balls to initialise.
 * \param[in] num_balls The number of balls to initialise.
 * \param[in] layout The layout of the balls.
 * \param[in] callback The callback to call when a ball has a collision.
 * \param[in] table_dimensions The dimensions of the table (x, z).
 * \param[in] table_center The center of the table.
 * \param[in] cue_position The position of the cue ball on the table 0-1 (x, z).
 * \param[in] target_pos The position of the target ball on the table 0-1 (x, z).
 * \param[in] drop_height The height to drop the balls from.
 */
void initialise_pool_balls(
    PhysWorld*  world,
    pool_ball** balls_ptr,
    int         num_balls,
    int         layout,
    void (*callback)(unsigned, unsigned),
    const ac_vec2* table_dimensions,
    const ac_vec3* table_center,
    const ac_vec2* cue_position,
    const ac_vec2* target_pos,
    const float    drop_height
);
/**
 * \brief Initialises the pool table.
 * \param[out] world The physics world to add the table to.
 * \param[out] table The table to initialise.
 */
void    initialise_pool_table(PhysWorld* world, pool_table* table);
/**
 * \brief Initialises the cue stick.
 * \param[out] stick The cue stick to initialise.
 */
void    initialise_cue_stick(cue_stick* stick);
/**
 * \brief Helper to calculate the world position of a ball based on the start position and table.
 * \param[in] start_position The position of the ball on the table 0-1 (x, z).
 * \param[in] surface_center The center of the table.
 * \param[in] surface_dimensions The dimensions of the table (x, z).
 * \param[in] drop_height The height to drop the balls from.
 */
ac_vec3 ball_start_pos_to_world_pos(
    const ac_vec2* start_position,
    const ac_vec3* surface_center,
    const ac_vec2* surface_dimensions,
    float          drop_height
);
