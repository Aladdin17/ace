/**
 * \file
 * \author Christien Alden
 * \author Blake Caldwell
 * \brief Defines the rendering functions for the application
 */
#pragma once
#include <stdbool.h>

//--------------------------------------------------------------------------------------------------
// Opaque types
//--------------------------------------------------------------------------------------------------

typedef struct pool_ball  pool_ball;
typedef union ac_vec3     ac_vec3;
typedef struct cue_stick  cue_stick;
typedef struct pool_app   pool_app;
typedef struct pool_table pool_table;
typedef struct PhysWorld  PhysWorld;

/**
 * \brief Draws a pool ball at the given position
 * \param ball The ball to draw
 * \param position The position to draw the ball at
 */
void draw_pool_ball(const pool_ball* ball, const ac_vec3* position);
/**
 * \brief Draws a cue stick at the given position
 * \param stick The stick to draw
 * \param position The position to draw the stick at
 * \param radius The radius of the target ball
 */
void draw_cue_stick(const cue_stick* stick, const ac_vec3* position, float radius);
/**
 * \brief Draws the pool table
 * \param table The table to draw
 * \param orthographic Whether to draw the table orthographically
 * \details
 * The \p orthographic parameter is used to determine whether to draw the table legs; as they are
 * not visible when the ortho camera is used.
 */
void draw_pool_table(const pool_table* table, bool orthographic);
/**
 * \brief Draws the scene
 * \param app The application to draw
 * \param orthographic Whether to draw the scene orthographically
 */
void draw_scene(const pool_app* app, bool orthographic);
/**
 * \brief Draws the scene in minimap mode
 * \param app The application to draw
 */
void draw_minimap(const pool_app* app);
/**
 * \brief Draws the power bar overlay
 * \param power_ratio The ratio of the power to draw, this should be in the range [0, 1]
 * \note \p power_ratio values will be clamped between [0, 1]
 */
void draw_powerbar(float power_ratio);
/**
 * \brief Draws the entity info overlay
 * \param world The physics world to draw the entity info from
 * \param entity_id The entity id to draw the info for
 */
void draw_entity_info(const PhysWorld* world, unsigned entity_id);
/**
 * \brief Draws the controls overlay
 */
void draw_controls_overlay(void);
/**
 * \brief Draws the initial screen
 * \details
 * This screen is displayed when the application is first started, it simply directs the
 * user to the console.
 */
void draw_init_screen(void);
