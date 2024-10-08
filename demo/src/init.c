/**
 * \file
 * \author Christien Alden
 * \author Blake Caldwell
 * \brief Initialisation functions for the pool app.
 */
#include "init.h"
#include "render.h"
#include "types.h"
#include <ace/geometry/shapes.h>
#include <ace/math/vec3_ext.h>
#include <ace/physics/phys_world.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//--------------------------------------------------------------------------------------------------
// Forward Declarations
//--------------------------------------------------------------------------------------------------

/**
 * \brief Sets up the balls in a triangle formation.
 * \param[in,out] balls The balls to set up.
 * \param[in] num_balls The number of balls to set up.
 * \param[in] world The physics world to add the balls to.
 * \param[in] start_pos The position to start the balls at.
 * \param[in] ball_radius The radius of the balls.
 */
void    ball_formation_triangle(pool_ball*, int, PhysWorld*, ac_vec3, float);
/**
 * \brief Sets up the balls in a rectangle formation.
 * \param[in,out] balls The balls to set up.
 * \param[in] num_balls The number of balls to set up.
 * \param[in] world The physics world to add the balls to.
 * \param[in] start_pos The position to start the balls at.
 * \param[in] ball_radius The radius of the balls.
 */
void    ball_formation_rectangle(pool_ball*, int, PhysWorld*, ac_vec3, float);
/**
 * \brief Generates a random mass for a ball between \p min and \p max.
 * \param[in] min The minimum mass.
 * \param[in] max The maximum mass.
 * \return The random mass.
 */
float   generate_random_ball_mass(float, float);
/**
 * \brief Lerps between blue (light) and red (heavy) ball based on its mass.
 * \param[in] mass The mass of the ball
 * \param[in] min The minimum mass.
 * \param[in] max The maximum mass.
 * \return The color of the ball.
 * \details The color of the ball is lerped between
 * blue and red based on the mass of the ball and the min and max mass. Meaning that a ball at
 * miniumum mass will be blue and a ball at maximum mass will be red.
 */
ac_vec3 generate_ball_color(float, float, float);

//--------------------------------------------------------------------------------------------------
// Misc
//--------------------------------------------------------------------------------------------------

void initialise_misc(pool_app* app)
{
    app->show_minimap          = true;
    app->show_entity_info      = true;
    app->show_controls         = true;
    app->min_ball_speed        = 0.07f;
    app->y_threshold           = -2.0f;
    app->cue_start_position    = (ac_vec2){ 0.0f, 0.70f };
    app->target_start_position = (ac_vec2){ 0.0f, -0.15f };
    app->ball_drop_height      = 0.2f;
}

//--------------------------------------------------------------------------------------------------
// Frame Time
//--------------------------------------------------------------------------------------------------

void initialise_frame_time(frame_time* time)
{
    // don't change this
    time->update_rate        = 120;
    time->last_frame_time    = 0;
    time->current_frame_time = 0;
}

//--------------------------------------------------------------------------------------------------
// Orbit Camera
//--------------------------------------------------------------------------------------------------

void initialise_orbit_camera(orbit_camera* camera)
{
    camera->radius          = 5.0f;
    camera->min_radius      = 2.0f;
    camera->max_radius      = 10.0f;
    camera->pitch_angle     = 30.0f;
    camera->min_pitch_angle = 10.0f;
    camera->max_pitch_angle = 60.0f;
    camera->yaw_angle       = 90.0f;
    camera->rotation_step   = 2.0f;
    camera->zoom_step       = 0.2f;
    camera->target          = ac_vec3_zero();
}

//--------------------------------------------------------------------------------------------------
// Physics
//--------------------------------------------------------------------------------------------------

void initialise_physics_world(PhysWorld* world, int update_rate)
{
    // force an update rate of 120 if the user tries to set it to 0 or less
    if ( update_rate <= 0 )
    {
        update_rate = 120;
    }

    phys_init_world(world);
    world->timeStep = 1.0f / update_rate;
}

//--------------------------------------------------------------------------------------------------
// Cue Stick
//--------------------------------------------------------------------------------------------------

void initialise_cue_stick(cue_stick* stick)
{
    stick->target_ball       = 0;
    stick->pitch_angle       = 0.0f;
    stick->min_pitch_angle   = 0.0f;
    stick->max_pitch_angle   = 90.0f;
    stick->yaw               = 0.0f;
    stick->power             = 0.0f;
    stick->power_step        = 0.05f;
    stick->max_power_newtons = 150.0f;
    stick->rotation_step     = 2.0f;
    stick->color             = (ac_vec3){ 0.651f, 0.51f, 0.035f };
    stick->length            = 1.45f;
    stick->radius            = 0.01f;
    stick->draw_distance     = 0.1f;
    stick->draw              = draw_cue_stick;
}

//--------------------------------------------------------------------------------------------------
// Pool Table
//--------------------------------------------------------------------------------------------------

void initialise_pool_table(PhysWorld* world, pool_table* table)
{
    // table dimensions
    static const ac_vec3 table_origin               = { 0.0f, 0.0f, 0.0f };
    static const ac_vec3 table_top_collider_origin  = { 0.0f, -0.025f, 0.0f };
    static ac_vec3       table_top_half_extents     = { 0.455f, 0.025f, 0.91f };
    static ac_vec3       long_cushion_half_extents  = { 0.05f, 0.05f, 0.96f };
    static ac_vec3       short_cushion_half_extents = { 0.46f, 0.05f, 0.05f };

    // initialise the table object
    table->surface_center = table_origin;
    table->surface_color  = (ac_vec3){ 0.1f, 0.5f, 0.1f };
    table->cushion_color  = (ac_vec3){ 0.65f, 0.33f, 0.16f };
    table->leg_color      = (ac_vec3){ 0.55f, 0.23f, 0.06f };
    table->length         = 1.82f;                  // 0.91m half-length
    table->width          = 0.91f;                  // 0.455m half-width
    table->top_depth      = 0.05f;                  // 0.015m half-depth
    table->cushion_width  = 0.10f;                  // 0.05m half-width
    table->cushion_height = 0.10f;                  // 0.05m half-height
    table->pocket_radius  = table->width * 0.055f;  // Approximately 5.5% of the table width

    // add the table surface
    unsigned table_top_id = phys_add_entity(world, &table_top_collider_origin);
    phys_make_entity_static(world, table_top_id);
    phys_add_entity_collider(
        world,
        (Collider){ .type = AABB_C, .data = &table_top_half_extents },
        table_top_id
    );
    table->physics_ids[0] = table_top_id;

    // long cushions (z-axis)
    // positive z
    table->cushion_centers[0] =
        (ac_vec3){ .x = table_origin.x + (table->width / 2.0f) + (table->cushion_width / 2.0f),
                   .y = table_origin.y,
                   .z = table_origin.z };

    // negative z
    table->cushion_centers[1] =
        (ac_vec3){ .x = table_origin.x - (table->width / 2.0f) - (table->cushion_width / 2.0f),
                   .y = table_origin.y,
                   .z = table_origin.z };

    // short cushions (x-axis)
    // positive x
    table->cushion_centers[2] =
        (ac_vec3){ .x = table_origin.x,
                   .y = table_origin.y,
                   .z = table_origin.z + (table->length / 2.0f) + table->cushion_width / 2.0f };

    // negative x
    table->cushion_centers[3] =
        (ac_vec3){ .x = table_origin.x,
                   .y = table_origin.y,
                   .z = table_origin.z - (table->length / 2.0f) - table->cushion_width / 2.0f };

    unsigned long_cushion_pos_id = phys_add_entity(world, &table->cushion_centers[0]);
    phys_make_entity_static(world, long_cushion_pos_id);
    phys_add_entity_collider(
        world,
        (Collider){ .type = AABB_C, .data = &long_cushion_half_extents },
        long_cushion_pos_id
    );
    table->physics_ids[1] = long_cushion_pos_id;

    unsigned long_cushion_neg_id = phys_add_entity(world, &table->cushion_centers[1]);
    phys_make_entity_static(world, long_cushion_neg_id);
    phys_add_entity_collider(
        world,
        (Collider){ .type = AABB_C, .data = &long_cushion_half_extents },
        long_cushion_neg_id
    );
    table->physics_ids[2] = long_cushion_neg_id;

    unsigned short_cushion_pos_id = phys_add_entity(world, &table->cushion_centers[2]);
    phys_make_entity_static(world, short_cushion_pos_id);
    phys_add_entity_collider(
        world,
        (Collider){ .type = AABB_C, .data = &short_cushion_half_extents },
        short_cushion_pos_id
    );
    table->physics_ids[3] = short_cushion_pos_id;

    unsigned short_cushion_neg_id = phys_add_entity(world, &table->cushion_centers[3]);
    phys_make_entity_static(world, short_cushion_neg_id);
    phys_add_entity_collider(
        world,
        (Collider){ .type = AABB_C, .data = &short_cushion_half_extents },
        short_cushion_neg_id
    );
    table->physics_ids[4] = short_cushion_neg_id;

    // add pockets

    float pocket_height = table_origin.y + 0.001f;
    //-x, -z
    table->pocket_centers[0] =
        (ac_vec3){ .x = table_origin.x - (table->width / 2.0f) + (table->pocket_radius),
                   .y = pocket_height,
                   .z = table_origin.z - (table->length / 2.0f) + (table->pocket_radius) };

    //-x, z
    table->pocket_centers[1] =
        (ac_vec3){ .x = table_origin.x - (table->width / 2.0f) + (table->pocket_radius),
                   .y = pocket_height,
                   .z = table_origin.z + (table->length / 2.0f) - (table->pocket_radius) };

    // x, z
    table->pocket_centers[2] =
        (ac_vec3){ .x = table_origin.x + (table->width / 2.0f) - (table->pocket_radius),
                   .y = pocket_height,
                   .z = table_origin.z + (table->length / 2.0f) - (table->pocket_radius) };

    // x, -z
    table->pocket_centers[3] =
        (ac_vec3){ .x = table_origin.x + (table->width / 2.0f) - (table->pocket_radius),
                   .y = pocket_height,
                   .z = table_origin.z - (table->length / 2.0f) + (table->pocket_radius) };

    for ( int i = 0; i < 4; ++i )
    {
        unsigned pocket_phys_id = phys_add_entity(world, &table->pocket_centers[i]);
        phys_make_entity_static(world, pocket_phys_id);
        phys_add_entity_collider(
            world,
            (Collider){ .type = SPHERE_C, .data = &table->pocket_radius },
            pocket_phys_id
        );
        table->pocket_physics_ids[i] = pocket_phys_id;
    }

    table->leg_length = short_cushion_half_extents.x;
    float leg_height  = table_origin.y - table->leg_length / 2.0f;

    //-x, -z
    table->leg_centers[0] = (ac_vec3){ .x = table_origin.x - (table->width / 2.0f),
                                       .y = leg_height,
                                       .z = table_origin.z - (table->length / 2.0f) };

    //-x, z
    table->leg_centers[1] = (ac_vec3){ .x = table_origin.x - (table->width / 2.0f),
                                       .y = leg_height,
                                       .z = table_origin.z + (table->length / 2.0f) };

    // x, z
    table->leg_centers[2] = (ac_vec3){ .x = table_origin.x + (table->width / 2.0f),
                                       .y = leg_height,
                                       .z = table_origin.z + (table->length / 2.0f) };

    // x, -z
    table->leg_centers[3] = (ac_vec3){ .x = table_origin.x + (table->width / 2.0f),
                                       .y = leg_height,
                                       .z = table_origin.z - (table->length / 2.0f) };

    table->draw = draw_pool_table;
}

//--------------------------------------------------------------------------------------------------
// Pool Balls
//--------------------------------------------------------------------------------------------------

float generate_random_ball_mass(float min, float max)
{
    float scale = rand() / (float) RAND_MAX;
    return min + scale * (max - min);
}

ac_vec3 generate_ball_color(float mass, float min, float max)
{
    // get the scale of the mass between min and max
    float   scale       = (mass - min) / (max - min);
    // get the color between red and green
    ac_vec3 start_color = { 0.0f, 0.0f, 1.0f };
    ac_vec3 end_color   = { 1.0f, 0.0f, 0.0f };
    return ac_vec3_lerp(&start_color, &end_color, scale);
}

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
)
{
    // get the number of balls from the user
    *balls_ptr       = (pool_ball*) malloc(num_balls * sizeof(pool_ball));
    pool_ball* balls = *balls_ptr;

    // currently all balls have the same radius but can differ in mass
    static const float    radius          = 0.0305f;
    static Sphere         sphere_collider = { .radius = 0.0305f };
    static const Collider collider        = { .type = SPHERE_C, .data = &sphere_collider };

    // cue ball
    ac_vec3 cue_start_pos =
        ball_start_pos_to_world_pos(cue_position, table_center, table_dimensions, drop_height);
    unsigned ball_id = phys_add_entity(world, &cue_start_pos);
    phys_add_entity_collider(world, collider, ball_id);
    phys_make_entity_dynamic(world, ball_id);
    phys_add_collision_callback(world, ball_id, callback);
    world->masses[ball_id] = 0.170f;

    balls[0].physics_id = ball_id;
    balls[0].color      = (ac_vec3){ 1.0f, 1.0f, 1.0f };
    balls[0].radius     = sphere_collider.radius;
    balls[0].draw       = draw_pool_ball;

    // seed the random number generator
    srand((unsigned int) time(NULL));
    for ( int i = 1; i < num_balls; ++i )
    {
        unsigned ball_index = phys_add_entity(world, &(ac_vec3){ 0.0f, 0.0f, 0.0f });
        phys_add_entity_collider(world, collider, ball_index);
        phys_make_entity_dynamic(world, ball_index);
        world->masses[ball_index] = generate_random_ball_mass(0.1f, 0.2f);

        balls[i].physics_id = ball_index;
        balls[i].color      = generate_ball_color(world->masses[ball_index], 0.1f, 0.2f);
        balls[i].radius     = radius;
        phys_add_collision_callback(world, ball_index, callback);
        balls[i].draw = draw_pool_ball;
    }

    ac_vec3 target_start_pos =
        ball_start_pos_to_world_pos(target_pos, table_center, table_dimensions, drop_height);

    // - 1 because of cueball
    switch ( layout )
    {
    case ball_layout_triangle:
        ball_formation_triangle(balls, num_balls - 1, world, target_start_pos, radius);
        break;
    case ball_layout_rectangle:
        ball_formation_rectangle(balls, num_balls - 1, world, target_start_pos, radius);
        break;
    default:
        // not ideal but without a logging library this will do
        printf("Invalid ball layout\n");
        exit(1);
    }
}

void ball_formation_triangle(
    pool_ball* balls, int num_balls, PhysWorld* world, ac_vec3 start_pos, float ball_radius
)
{
    const float spacing_modifier = 1.8f;
    float       spacing          = ball_radius * spacing_modifier;
    int         ball_index       = 1;
    int         row              = 0;
    int         curr_row_len     = 1;

    while ( ball_index <= num_balls )  // while more balls to place
    {
        for ( int i = 0; i < curr_row_len && ball_index <= num_balls;
              i++ )  // set row by row till balls run out
        {
            ac_vec3 pos = {
                .x = start_pos.x +
                     (i - (curr_row_len - 1) / 2.0f) *
                         spacing,  // will set first ball depending on num balls and spacing
                .y = start_pos.y,
                .z = start_pos.z - row * spacing  // move down -z
            };

            world->positions[balls[ball_index].physics_id] = pos;

            ball_index++;
        }

        row++;
        curr_row_len++;
    }
}

void ball_formation_rectangle(
    pool_ball* balls, int num_balls, PhysWorld* world, ac_vec3 start_pos, float ball_radius
)
{
    float spacing       = ball_radius * 1.8f;
    int   ball_index    = 1;
    int   row           = 0;
    int   col           = 0;
    int   balls_per_row = (num_balls / (int) sqrtf((float) num_balls)) - 1;

    while ( ball_index <= num_balls )
    {
        ac_vec3 pos = {
            .x = start_pos.x +
                 (col - balls_per_row / 2.0f) *
                     spacing,  // shifts so positions around start_pos (not like triangle)
            .y = start_pos.y,
            .z = start_pos.z - row * spacing
        };

        world->positions[balls[ball_index].physics_id] = pos;

        ball_index++;

        col++;
        if ( col >= balls_per_row )
        {
            row++;
            col = 0;
        }
    }
}

//--------------------------------------------------------------------------------------------------
// Utility
//--------------------------------------------------------------------------------------------------

ac_vec3 ball_start_pos_to_world_pos(
    const ac_vec2* start_position,
    const ac_vec3* surface_center,
    const ac_vec2* surface_dimensions,
    float          drop_height
)
{
    return (ac_vec3){ .x = surface_center->x + surface_dimensions->x / 2.0f * start_position->x,
                      .y = surface_center->y + drop_height,
                      .z = surface_center->z + surface_dimensions->y / 2.0f * start_position->y };
}
