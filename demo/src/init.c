#include "init.h"
#include "types.h"
#include "render.h"
#include <ace/physics/phys_world.h>
#include <ace/geometry/shapes.h>
#include <string.h>

//--------------------------------------------------------------------------------------------------
// Orbit Camera
//--------------------------------------------------------------------------------------------------

void initialise_orbit_camera( orbit_camera* camera )
{
    camera->radius = 5.0f;
    camera->pitch_angle = 30.0f;
    camera->yaw_angle = 90.0f;
    camera->target = (ac_vec3){0, 0, 0};
}

//--------------------------------------------------------------------------------------------------
// Physics
//--------------------------------------------------------------------------------------------------

void initialise_physics_world( PhysWorld* world, int update_rate )
{
    phys_init_world(world);
    world->timeStep = 1.0f / update_rate;
}

//--------------------------------------------------------------------------------------------------
// Pool Table
//--------------------------------------------------------------------------------------------------

void initialise_pool_table( PhysWorld* world, pool_table* table )
{
    // we need colliders for the table-top and the cushions
    // the dimensions are going to be hard-coded for now
    // for a table of 2.24m x 1.12m and a height of 10cm
    // with the long side aligned with the z-axis
    static const ac_vec3 tableTopHalfExtents = { 0.56f, 0.05f, 1.12f };
    // static const ac_vec3 longCushionHalfExtents = { 0.05f, 0.10f, 1.12f };
    // // added 0.1f to the width to ensure no balls get stuck in the corners
    // static const ac_vec3 shortCushionHalfExtents = { 0.66f, 0.10f, 0.05f };


    const ac_vec3 table_top_pos = { 0.0f, -0.06f, 0.0f };

    // add the table top
    unsigned table_top_id = phys_add_entity(world, &table_top_pos);
    phys_make_entity_static(world, table_top_id);
    phys_add_entity_collider(world, (Collider){.type = AABB_C, .data = &tableTopHalfExtents}, table_top_id);

    // add the cushions
    // unsigned long_cushion_id = phys_add_entity(world, &table_top_pos);
    // phys_make_entity_static(world, long_cushion_id);



    // unsigned tableID = phys_add_entity(world, &(ac_vec3){ 0.0f, 0.0f, 0.0f});
    // phys_make_entity_static(world, tableID);

//     tableTopCollider.type = AABB_C;
//     tableTopCollider.data = &tableTopHalfExtents;

//     phys_add_entity_collider(&physicsWorld, tableTopCollider, tableTopID);
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
    { {1.0f, 1.0f, 1.0f}, "Cue", 0.170f },
    { {0.5f, 0.0f, 0.0f},   "1", 0.170f },
    { {0.0f, 1.0f, 0.0f},   "2", 0.110f },
    { {1.0f, 1.0f, 0.0f},   "3", 0.130f },
    { {1.0f, 0.0f, 1.0f},   "4", 0.150f },
    { {1.0f, 0.0f, 0.0f},   "5", 0.100f },
    { {0.0f, 0.0f, 1.0f},   "6", 0.120f },
    { {0.0f, 1.0f, 1.0f},   "7", 0.140f },
    { {0.0f, 0.5f, 0.0f},   "8", 0.180f },
    { {0.0f, 0.0f, 0.5f},   "9", 0.190f },
    { {0.5f, 0.5f, 0.5f},  "10", 0.160f }
};

void initialise_pool_balls( PhysWorld* world, pool_ball* balls, int* num_balls )
{
    // currently all balls have the same radius but can differ in mass
    static const float radius = 0.0305f;
    static Sphere sphere_collider = { .radius = 0.0305f };
    static const Collider collider = { .type = SPHERE_C, .data = &sphere_collider };

    *num_balls = 11;

    // cue ball
    unsigned ball_id = phys_add_entity(world, &(ac_vec3){ 0.0f, 0.0f, 0.85f });
    phys_add_entity_collider(world, collider, ball_id);
    phys_make_entity_dynamic(world, ball_id);
    // phys_add_collision_callback(world, ballID, collisionCallback);
    world->masses[ball_id] = ball_setup[0].mass;

    ball_info* info = &ball_setup[0];
    balls[0].physics_id = ball_id;
    balls[0].color = info->color;
    balls[0].radius = sphere_collider.radius;
    strcpy(balls[0].tag, info->tag);
    balls[0].draw = draw_pool_ball; // draw_ball;

    // Set up pool ball formation
    int num_rows = 4;
    float spacing = radius * 1.8f;
    float start_z = 0.0f;
    int ball_index = 1;

    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col <= row; ++col) {
            ac_vec3 pos = (ac_vec3){
                .x = (col - row / 2.0f) * spacing,
                .y = 0.0f,
                .z = start_z - row * spacing
            };

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

void initialise_cue_stick( cue_stick* stick )
{
    stick->pitch = 0.0f;
    stick->yaw = 0.0f;
    stick->power = 0.0f;
    stick->target = 0;  // cue ball
    stick->strike = false;
    stick->visible = true;
    stick->draw = draw_cue_stick;
}
