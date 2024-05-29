#include "init.h"
#include "types.h"
#include "render.h"
#include <ace/physics/phys_world.h>
#include <ace/geometry/shapes.h>
#include <string.h>

//--------------------------------------------------------------------------------------------------
// Orbit Camera
//--------------------------------------------------------------------------------------------------

void initialise_orbit_camera(orbit_camera *camera)
{
    camera->radius = 5.0f;
    camera->pitch_angle = 30.0f;
    camera->yaw_angle = 90.0f;
    camera->target = (ac_vec3){0, 0, 0};
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
    {{0.0f, 0.0f, 1.0f}, "6", 0.120f},
    {{0.0f, 1.0f, 1.0f}, "7", 0.140f},
    {{0.0f, 0.5f, 0.0f}, "8", 0.180f},
    {{0.0f, 0.0f, 0.5f}, "9", 0.190f},
    {{0.5f, 0.5f, 0.5f}, "10", 0.160f}};

void initialise_pool_balls(PhysWorld *world, pool_ball *balls, int *num_balls)
{
    // currently all balls have the same radius but can differ in mass
    static const float radius = 0.0305f;
    static Sphere sphere_collider = {.radius = 0.0305f};
    static const Collider collider = {.type = SPHERE_C, .data = &sphere_collider};

    *num_balls = 11;

    // cue ball
    unsigned ball_id = phys_add_entity(world, &(ac_vec3){0.0f, 0.2f, 0.55f});
    phys_add_entity_collider(world, collider, ball_id);
    phys_make_entity_dynamic(world, ball_id);
    // phys_add_collision_callback(world, ballID, collisionCallback);
    world->masses[ball_id] = ball_setup[0].mass;

    ball_info *info = &ball_setup[0];
    balls[0].physics_id = ball_id;
    balls[0].color = info->color;
    balls[0].radius = sphere_collider.radius;
    strcpy(balls[0].tag, info->tag);
    balls[0].draw = draw_pool_ball; // draw_ball;

    // Set up pool ball formation
    int num_rows = 4;
    float spacing = radius * 1.8f;
    float start_z = -0.455f;
    int ball_index = 1;

    for (int row = 0; row < num_rows; ++row)
    {
        for (int col = 0; col <= row; ++col)
        {
            ac_vec3 pos = (ac_vec3){
                .x = (col - row / 2.0f) * spacing,
                .y = 0.2f,
                .z = start_z - row * spacing};

            ball_id = phys_add_entity(world, &pos);
            phys_add_entity_collider(world, collider, ball_id);
            phys_make_entity_dynamic(world, ball_id);
            // phys_add_collision_callback(world, ballIDs[ballIndex], collisionCallback);
            world->masses[ball_id] = ball_setup[ball_index].mass;

            // add to app structure
            info = &ball_setup[ball_index];
            balls[ball_index].physics_id = ball_id;
            balls[ball_index].color = info->color;
            balls[ball_index].radius = sphere_collider.radius;
            strcpy(balls[ball_index].tag, info->tag);
            balls[ball_index].draw = draw_pool_ball; // draw_ball;
            ball_index++;
        }
    }
}

void initialise_cue_stick(cue_stick *stick)
{
    stick->pitch = 0.0f;
    stick->yaw = 0.0f;
    stick->power = 0.0f;
    stick->target = 0; // cue ball
    stick->strike = false;
    stick->visible = true;
    stick->draw = draw_cue_stick;
}
