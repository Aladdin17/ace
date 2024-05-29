#include "init.h"
#include "types.h"
#include "render.h"
#include <ace/physics/phys_world.h>
#include <ace/geometry/shapes.h>

//--------------------------------------------------------------------------------------------------
// Orbit Camera
//--------------------------------------------------------------------------------------------------

void initialise_orbit_camera( orbit_camera* camera )
{
    camera->radius = 2.0f;
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
    // not implemented

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
    ac_vec3 pos;
    ac_vec3 color;
    char tag[16];
    float mass;
    float radius;
} ball_info;

static ball_info ball_setup[] = {
    { {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, "Cue", 0.170f, 0.05715f},
    { {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},   "1", 0.170f, 0.05715f},
    { {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},   "2", 0.170f, 0.05715f},
    { {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},   "3", 0.170f, 0.05715f},
    { {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},   "4", 0.170f, 0.05715f},
    { {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},   "5", 0.170f, 0.05715f},
    { {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},   "6", 0.170f, 0.05715f},
    { {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},   "7", 0.170f, 0.05715f},
    { {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},   "8", 0.170f, 0.05715f},
    { {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},   "9", 0.170f, 0.05715f},
    { {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},  "10", 0.170f, 0.05715f}
};

void initialise_pool_balls( PhysWorld* world, pool_ball* balls, int* num_balls )
{
    // currently all balls have the same radius but can differ in mass
    static Sphere sphere_collider = { .radius = 0.0305f  };
    static const Collider collider = { .type = SPHERE_C, .data = &sphere_collider };

    *num_balls = 11;
    for (int i = 0; i < *num_balls; ++i)
    {
        ball_info* info = &ball_setup[i];

        // add to physics world
        unsigned ball_id = phys_add_entity(world, &info->pos);
        phys_add_entity_collider(world, collider, ball_id);
        phys_make_entity_dynamic(world, ball_id);
        // phys_add_collision_callback(world, ballID, collisionCallback);
        world->masses[ball_id] = info->mass;

        // add to app structure
        balls[i].physics_id = ball_id;
        balls[i].color = info->color;
        balls[i].radius = info->radius;
        strcpy(balls[i].tag, info->tag);
        balls[i].draw = draw_pool_ball; // draw_ball;
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
