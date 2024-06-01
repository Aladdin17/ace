#include "init.h"
#include "types.h"
#include "render.h"
#include <ace/physics/phys_world.h>
#include <ace/geometry/shapes.h>
#include <string.h>
#include <stdlib.h>

//--------------------------------------------------------------------------------------------------
// Misc
//--------------------------------------------------------------------------------------------------

void initialise_misc( pool_app* app )
{
    app->show_minimap = false;
    app->show_entity_info = false;
}

//--------------------------------------------------------------------------------------------------
// Frame Time
//--------------------------------------------------------------------------------------------------

void initialise_frame_time(frame_time *time)
{
    time->update_rate = 120;
    time->last_frame_time = 0;
    time->current_frame_time = 0;
}

//--------------------------------------------------------------------------------------------------
// Orbit Camera
//--------------------------------------------------------------------------------------------------

void initialise_orbit_camera(orbit_camera *camera)
{
    camera->radius = 5.0f;
    camera->min_radius = 2.0f;
    camera->max_radius = 10.0f;
    camera->pitch_angle = 30.0f;
    camera->min_pitch_angle = 10.0f;
    camera->max_pitch_angle = 60.0f;
    camera->yaw_angle = 90.0f;
    camera->rotation_step = 1.0f;
    camera->zoom_step = 0.2f;
    camera->target = ac_vec3_zero();
}

//--------------------------------------------------------------------------------------------------
// Physics
//--------------------------------------------------------------------------------------------------

void initialise_physics_world(PhysWorld *world, int update_rate)
{
    phys_init_world(world);
    world->timeStep = 1.0f / update_rate;
}

//--------------------------------------------------------------------------------------------------
// Pool Table
//--------------------------------------------------------------------------------------------------

void initialise_pool_table(PhysWorld *world, pool_table *table)
{
    // with the long side aligned with the z-axis
    static const ac_vec3 table_origin = {0.0f, 0.0f, 0.0f};
    static const ac_vec3 table_top_collider_origin = {0.0f, -0.025f, 0.0f};
    static const ac_vec3 table_top_half_extents = {0.455f, 0.025f, 0.91f};
    static const ac_vec3 long_cushion_half_extents = {0.05f, 0.05f, 0.96f};
    static const ac_vec3 short_cushion_half_extents = {0.46f, 0.05f, 0.05f};

    // static const ac_vec3 table_top_half_extents = {table->width / 2.0f, table->top_depth / 2.0f, table->length / 2.0f};
    // static const ac_vec3 table_top_collider_origin = {0.0f, 0.0f, 0.0f};
    // static const ac_vec3 long_cushion_half_extents = {table->cushion_width / 2.0f, table->cushion_height / 2.0f, table->length / 2.0f + table->cushion_width};
    // static const ac_vec3 short_cushion_half_extents = {table->width / 2.0f + table->cushion_width, table->cushion_height / 2.0f, table->cushion_width / 2.0f};

    table->length = 1.82f;          // 0.91m half-length
    table->width = 0.91f;           // 0.455m half-width
    table->top_depth = 0.05f;       // 0.015m half-depth
    table->cushion_width = 0.10f;   // 0.05m half-width
    table->cushion_height = 0.10f;  // 0.05m half-height

    table->pocket_radius = table->width * 0.055; // Pocket radius is approximately 5.5% of the table width


    // add the table top
    unsigned table_top_id = phys_add_entity(world, &table_top_collider_origin);
    phys_make_entity_static(world, table_top_id);
    phys_add_entity_collider(world, (Collider){.type = AABB_C, .data = &table_top_half_extents}, table_top_id);
    table->physics_ids[0] = table_top_id;

    //long cushions (z-axis)
    //positive z
    table->cushion_centers[0] = (ac_vec3){.x = table_origin.x + (table->width / 2.0f) + (table->cushion_width / 2.0f),
                                          .y = table_origin.y,
                                          .z = table_origin.z};

    // negative z
    table->cushion_centers[1] = (ac_vec3){.x = table_origin.x - (table->width / 2.0f) - (table->cushion_width / 2.0f),
                                          .y = table_origin.y,
                                          .z = table_origin.z};

    // short cushions (x-axis)
    // positive x
    table->cushion_centers[2] = (ac_vec3){.x = table_origin.x,
                                          .y = table_origin.y,
                                          .z = table_origin.z + (table->length / 2.0f) + table->cushion_width / 2.0f};

    // negative x
    table->cushion_centers[3] = (ac_vec3){.x = table_origin.x,
                                          .y = table_origin.y,
                                          .z = table_origin.z - (table->length / 2.0f) - table->cushion_width / 2.0f};



    unsigned long_cushion_pos_id = phys_add_entity(world, &table->cushion_centers[0]);
    phys_make_entity_static(world, long_cushion_pos_id);
    phys_add_entity_collider(world, (Collider){.type = AABB_C, .data = &long_cushion_half_extents}, long_cushion_pos_id);
    table->physics_ids[1] = long_cushion_pos_id;

    unsigned long_cushion_neg_id = phys_add_entity(world, &table->cushion_centers[1]);
    phys_make_entity_static(world, long_cushion_neg_id);
    phys_add_entity_collider(world, (Collider){.type = AABB_C, .data = &long_cushion_half_extents}, long_cushion_neg_id);
    table->physics_ids[2] = long_cushion_neg_id;

    unsigned short_cushion_pos_id = phys_add_entity(world, &table->cushion_centers[2]);
    phys_make_entity_static(world, short_cushion_pos_id);
    phys_add_entity_collider(world, (Collider){.type = AABB_C, .data = &short_cushion_half_extents}, short_cushion_pos_id);
    table->physics_ids[3] = short_cushion_pos_id;

    unsigned short_cushion_neg_id = phys_add_entity(world, &table->cushion_centers[3]);
    phys_make_entity_static(world, short_cushion_neg_id);
    phys_add_entity_collider(world, (Collider){.type = AABB_C, .data = &short_cushion_half_extents}, short_cushion_neg_id);
    table->physics_ids[4] = short_cushion_neg_id;


    // add pockets

    float pocket_height = table_origin.y + 0.001f;
    //-x, -z
    table->pocket_centers[0] = (ac_vec3){ .x = table_origin.x - (table->width / 2.0f) + (table->pocket_radius),
                                          .y = pocket_height,
                                          .z = table_origin.z - (table->length / 2.0f) + (table->pocket_radius)};

    //-x, z
    table->pocket_centers[1] = (ac_vec3){ .x = table_origin.x - (table->width / 2.0f) + (table->pocket_radius),
                                          .y = pocket_height,
                                          .z = table_origin.z + (table->length / 2.0f) - (table->pocket_radius)};

    //x, z
    table->pocket_centers[2] = (ac_vec3){ .x = table_origin.x + (table->width / 2.0f) - (table->pocket_radius),
                                          .y = pocket_height,
                                          .z = table_origin.z + (table->length / 2.0f) - (table->pocket_radius)};

    //x, -z
    table->pocket_centers[3] = (ac_vec3){ .x = table_origin.x + (table->width / 2.0f) - (table->pocket_radius),
                                          .y = pocket_height,
                                          .z = table_origin.z - (table->length / 2.0f) + (table->pocket_radius)};


    for (int i = 0; i < 4; ++i)
    {
        unsigned pocket_phys_id = phys_add_entity(world, &table->pocket_centers[i]);
        phys_make_entity_static(world, pocket_phys_id);
        phys_add_entity_collider(world, (Collider){.type = SPHERE_C, .data = &table->pocket_radius}, pocket_phys_id);
        table->pocket_physics_ids[i] = pocket_phys_id;
    }

    table->leg_length = short_cushion_half_extents.x;
    float leg_height = table_origin.y - table->leg_length / 2.0f;

    //-x, -z
    table->leg_centers[0] = (ac_vec3){ .x = table_origin.x - (table->width / 2.0f),
                                    .y = leg_height,
                                    .z = table_origin.z - (table->length / 2.0f)};

    //-x, z
    table->leg_centers[1] = (ac_vec3){ .x = table_origin.x - (table->width / 2.0f),
                                    .y = leg_height,
                                    .z = table_origin.z + (table->length / 2.0f)};

    //x, z
    table->leg_centers[2] = (ac_vec3){ .x = table_origin.x + (table->width / 2.0f),
                                    .y = leg_height,
                                    .z = table_origin.z + (table->length / 2.0f)};

    //x, -z
    table->leg_centers[3] = (ac_vec3){ .x = table_origin.x + (table->width / 2.0f),
                                    .y = leg_height,
                                    .z = table_origin.z - (table->length / 2.0f)};


    table->draw = draw_pool_table;
}

//--------------------------------------------------------------------------------------------------
// Pool Balls
//--------------------------------------------------------------------------------------------------

typedef struct
{
    ac_vec3 color;
    char tag[16];
    float mass;
} ball_info;

static ball_info ball_setup[] = {
    {{1.0f, 1.0f, 1.0f}, "Cue", 0.170f},
    {{0.5f, 0.0f, 0.0f}, "1", 0.170f},
    {{0.0f, 1.0f, 0.0f}, "2", 0.110f},
    {{1.0f, 1.0f, 0.0f}, "3", 0.130f},
    {{1.0f, 0.0f, 1.0f}, "4", 0.150f},
    {{1.0f, 0.0f, 0.0f}, "5", 0.100f},
    {{0.1f, 0.2f, 0.7f}, "6", 0.120f},
    {{0.0f, 1.0f, 1.0f}, "7", 0.140f},
    {{0.1f, 0.2f, 0.7f}, "8", 0.180f},
    {{0.0f, 0.0f, 0.5f}, "9", 0.190f},
    {{0.5f, 0.5f, 0.5f}, "10", 0.160f},
    {{1.0f, 0.5f, 0.0f}, "11", 0.170f},
    {{0.5f, 1.0f, 0.5f}, "12", 0.110f},
    {{0.0f, 0.5f, 1.0f}, "13", 0.130f},
    {{0.5f, 0.0f, 1.0f}, "14", 0.150f},
    {{1.0f, 0.5f, 0.5f}, "15", 0.100f},
    {{0.5f, 1.0f, 0.0f}, "16", 0.120f},
    {{0.0f, 0.5f, 0.5f}, "17", 0.140f},
    {{0.5f, 0.0f, 0.5f}, "18", 0.180f},
    {{0.5f, 0.5f, 1.0f}, "19", 0.190f},
    {{1.0f, 0.0f, 0.5f}, "20", 0.160f},
    {{0.0f, 1.0f, 0.5f}, "21", 0.170f},
    {{0.5f, 0.5f, 0.0f}, "22", 0.110f},
    {{1.0f, 0.5f, 1.0f}, "23", 0.130f},
    {{0.5f, 1.0f, 1.0f}, "24", 0.150f},
    {{1.0f, 1.0f, 0.5f}, "25", 0.100f},
    {{0.5f, 0.0f, 0.0f}, "26", 0.120f},
    {{0.0f, 0.0f, 1.0f}, "27", 0.140f},
    {{1.0f, 0.0f, 1.0f}, "28", 0.180f},
    {{0.0f, 1.0f, 1.0f}, "29", 0.190f},
    {{0.5f, 0.5f, 0.5f}, "30", 0.160f}
};

void ball_formation_triangle(pool_ball *balls, int num_balls, PhysWorld* world, ac_vec3 start_pos, float ball_radius);
void ball_formation_rectangle(pool_ball *balls, int num_balls, PhysWorld* world, ac_vec3 start_pos, float ball_radius);


void initialise_pool_balls(PhysWorld *world, pool_ball **balls_ptr, int num_balls, int layout, void(*callback)(unsigned, unsigned))
{
    // get the number of balls from the user
    *balls_ptr = (pool_ball*)malloc(num_balls * sizeof(pool_ball));
    pool_ball *balls = *balls_ptr;

    // currently all balls have the same radius but can differ in mass
    static const float radius = 0.0305f;
    static Sphere sphere_collider = {.radius = 0.0305f};
    static const Collider collider = {.type = SPHERE_C, .data = &sphere_collider};

    // cue ball
    unsigned ball_id = phys_add_entity(world, &(ac_vec3){0.0f, 0.2f, 0.55f});
    phys_add_entity_collider(world, collider, ball_id);
    phys_make_entity_dynamic(world, ball_id);
    phys_add_collision_callback(world, ball_id, callback);
    world->masses[ball_id] = ball_setup[0].mass;

    ball_info *info = &ball_setup[0];
    balls[0].physics_id = ball_id;
    balls[0].color = info->color;
    balls[0].radius = sphere_collider.radius;
    balls[0].draw = draw_pool_ball; // draw_ball;

    ac_vec3 start_pos = {0, 0.2f,-0.455f};

    float ball_radius = 0.0305f;

    for (int i = 1; i < num_balls; ++i)
    {
        unsigned ball_index = phys_add_entity(world, &(ac_vec3){0.0f, 0.0f, 0.0f});
        phys_add_entity_collider(world, collider, ball_index);
        phys_make_entity_dynamic(world, ball_index);
        world->masses[ball_index] = ball_setup[i].mass;

        balls[i].physics_id = ball_index;
        balls[i].color = ball_setup[i].color;
        balls[i].radius = ball_radius;
        phys_add_collision_callback(world, ball_index, callback);
        balls[i].draw = draw_pool_ball;
    }

    // - 1 because of cueball
    switch (layout)
    {
    case ball_layout_triangle:
        ball_formation_triangle(balls, num_balls - 1, world, start_pos, ball_radius);
        break;
    case ball_layout_rectangle:
        ball_formation_rectangle(balls, num_balls - 1, world, start_pos, ball_radius);
        break;
    }
}

void initialise_cue_stick(cue_stick *stick)
{
    stick->target_ball = 0;
    stick->pitch_angle = 0.0f;
    stick->min_pitch_angle = 0.0f;
    stick->max_pitch_angle = 90.0f;
    stick->yaw = 0.0f;
    stick->power = 0.0f;
    stick->power_step = 0.05f;
    stick->max_power_ms = 5.0f;
    stick->rotation_step = 1.0f;
    stick->color = (ac_vec3){ 0.651f, 0.51f, 0.035f };
    stick->length = 1.45f;
    stick->radius = 0.01f;
    stick->draw_distance = 0.1f;
    stick->draw = draw_cue_stick;
}

void ball_formation_triangle(pool_ball *balls, int num_balls, PhysWorld* world, ac_vec3 start_pos, float ball_radius)
{
    float spacing = ball_radius * 1.8f;
    int ball_index = 1;
    int row = 0;
    int curr_row_len = 1;

    while (ball_index <= num_balls) //while more balls to place
    {
        for (int i = 0; i < curr_row_len && ball_index <= num_balls; i++) // set row by row till balls run out
        {
            ac_vec3 pos = {
                .x = start_pos.x + (i - (curr_row_len - 1) / 2.0f) * spacing, // will set first ball depending on num balls and spacing
                .y = start_pos.y,
                .z = start_pos.z - row * spacing //move down -z
            };

            world->positions[balls[ball_index].physics_id] = pos;

            ball_index++;
        }

        row++;
        curr_row_len++;
    }
}

void ball_formation_rectangle(pool_ball *balls, int num_balls, PhysWorld* world, ac_vec3 start_pos, float ball_radius)
{
    float spacing = ball_radius * 1.8f;
    int ball_index = 1;
    int row = 0;
    int col = 0;
    int balls_per_row = num_balls / 2; // (simple 2 rows)

    while (ball_index <= num_balls)
    {
        ac_vec3 pos = {
            .x = start_pos.x + (col - balls_per_row / 2.0f) * spacing, //shifts so positions around start_pos (not like triangle)
            .y = start_pos.y,
            .z = start_pos.z - row * spacing
        };

        world->positions[balls[ball_index].physics_id] = pos;

        ball_index++;

        col++;
        if (col >= balls_per_row)
        {
            row++;
            col = 0;
        }
    }
}