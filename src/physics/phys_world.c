#include "phys_world.h"
#include "memory.h"
#include "phys_collision.h"
#include "stdio.h"

void phys_init_world(PhysWorld* world)
{
    world->numEnts = world->numColliders = world->numDynamicEntities = world->numStaticEntities = 0;

    world->gravity.x          = 0.0f;
    world->gravity.z          = 0.0f;
    world->gravity.y          = -9.8f;
    world->airResistance      = 0.3f;
    world->velocityThreshhold = 0.075f;

    world->timeStep = 1.0f / 120.0f;

    memset(world->positions, 0, sizeof(void*) * AC_MAX_PHYS_ENTS);
    memset(world->velocities, 0, sizeof(ac_vec3) * AC_MAX_PHYS_ENTS);

    memset(world->masses, 1, sizeof(float) * AC_MAX_PHYS_ENTS);

    memset(world->callbacks, 0, sizeof(void*) * AC_MAX_PHYS_ENTS);

    memset(world->sleeping, 0, sizeof(bool) * AC_MAX_PHYS_ENTS);

    memset(world->colliders, 0, sizeof(Collider) * AC_MAX_PHYS_ENTS);

    memset(world->staticEntities, 0, sizeof(unsigned) * AC_MAX_PHYS_ENTS);
    memset(world->dynamicEntities, 0, sizeof(unsigned) * AC_MAX_PHYS_ENTS);
}

unsigned phys_add_entity(PhysWorld* world, ac_vec3* position)
{
    if ( world->numEnts < AC_MAX_PHYS_ENTS )
    {
        world->positions[world->numEnts] = position;

        world->velocities[world->numEnts].x = 0.0f;
        world->velocities[world->numEnts].y = 0.0f;
        world->velocities[world->numEnts].z = 0.0f;

        world->numEnts++;  // not the best - if in future i have time [fix]
        return world->numEnts - 1;
    }
    return AC_PHYS_ERROR_ENT;
}

void phys_add_entity_collider(PhysWorld* world, Collider collider, unsigned entity)
{
    if ( world->numColliders <= world->numEnts && entity <= world->numEnts )
    {
        world->colliders[entity] = collider;
        world->numColliders++;
    }
}

// NEED TO MAKE THEES MORE ROBUST

void phys_make_entity_dynamic(PhysWorld* world, unsigned entity)
{
    world->dynamicEntities[world->numDynamicEntities] = entity;
    world->numDynamicEntities++;
}

void phys_make_entity_static(PhysWorld* world, unsigned entity)
{
    world->staticEntities[world->numStaticEntities] = entity;
    world->numStaticEntities++;
}

void phys_add_collision_callback(PhysWorld* world, unsigned entity, PhysCallBack callback)
{
    world->callbacks[entity] = callback;
}

void phys_sleep_entity(PhysWorld* world, unsigned entity, bool sleep)
{
    world->sleeping[entity] = sleep;
    world->velocities[entity] = (ac_vec3){0,0,0};
}

// update funcs

void update_collisions(PhysWorld* world);
void update_movements(PhysWorld* world);

void phys_update(PhysWorld* world, float deltaTime)
{
    world->accumulator += deltaTime;

    while ( world->accumulator >= world->timeStep )
    {
        update_movements(world);
        update_collisions(world);
        world->accumulator -= world->timeStep;
    }
}

void update_collisions(PhysWorld* world)
{
    IntersectionResult result;

    unsigned entity1 = 0, entity2 = 0;
    for ( unsigned i = 0; i < world->numDynamicEntities; i++ )
    {
        entity1 = world->dynamicEntities[i];

        // check collisions between dynamic colliders
        for ( unsigned j = i + 1; j < world->numDynamicEntities; j++ )
        {
            entity2 = world->dynamicEntities[j];

            result = check_collision(
                &world->colliders[entity1],
                world->positions[entity1],
                &world->colliders[entity2],
                world->positions[entity2]
            );
            if ( result.intersected )
            {
                resolve_collision(
                    &result,
                    world->positions[entity1],
                    &world->velocities[entity1],
                    world->masses[entity1],
                    0,
                    world->positions[entity2],
                    &world->velocities[entity2],
                    world->masses[entity2],
                    0
                );

                if ( world->callbacks[entity1] )
                    world->callbacks[entity1](entity1, entity2);

                if ( world->callbacks[entity2] )
                    world->callbacks[entity2](entity1, entity2);
            }
        }

        // check collisions between dynamic and static colliders
        for ( unsigned j = 0; j < world->numStaticEntities; j++ )
        {
            entity2 = world->staticEntities[j];

            result = check_collision(
                &world->colliders[entity1],
                world->positions[entity1],
                &world->colliders[entity2],
                world->positions[entity2]
            );
            if ( result.intersected )
            {
                resolve_collision(
                    &result,
                    world->positions[entity1],
                    &world->velocities[entity1],
                    world->masses[entity1],
                    0,
                    world->positions[entity2],
                    &world->velocities[entity2],
                    world->masses[entity2],
                    1
                );

                if ( world->callbacks[entity1] )
                    world->callbacks[entity1](entity1, entity2);

                if ( world->callbacks[entity2] )
                    world->callbacks[entity2](entity1, entity2);
            }
        }
    }
}

void update_movements(PhysWorld* world)
{  // semi implicit euler
    for ( unsigned i = 0; i < world->numDynamicEntities; i++ )
    {
        unsigned entityIndex = world->dynamicEntities[i];

        if(world->sleeping[entityIndex])
            continue;

        // update positions based on velocity and half gravity * timeStep^2
        world->positions[entityIndex]->x +=
            world->velocities[entityIndex].x * world->timeStep +
            0.5f * world->gravity.x * world->timeStep * world->timeStep;  // gravity (x)

        world->positions[entityIndex]->y +=
            world->velocities[entityIndex].y * world->timeStep +
            0.5f * world->gravity.y * world->timeStep * world->timeStep;  // gravity (y)

        world->positions[entityIndex]->z +=
            world->velocities[entityIndex].z * world->timeStep +
            0.5f * world->gravity.z * world->timeStep * world->timeStep;  // gravity (z)

        // update velocities based on gravity and air resistance
        world->velocities[entityIndex].x += world->gravity.x * world->timeStep;  // gravity (x)
        world->velocities[entityIndex].y += world->gravity.y * world->timeStep;  // gravity (y)
        world->velocities[entityIndex].z += world->gravity.z * world->timeStep;  // gravity (z)

        world->velocities[entityIndex].x *= 1.0f - (world->airResistance * world->timeStep);
        world->velocities[entityIndex].y *= 1.0f - (world->airResistance * world->timeStep);
        world->velocities[entityIndex].z *= 1.0f - (world->airResistance * world->timeStep);

        //  if under the speed threshold, set velocity to 0
        if ( ac_vec3_magnitude(&world->velocities[entityIndex]) < world->velocityThreshhold )
        {
            world->velocities[entityIndex].x = 0.0f;
            world->velocities[entityIndex].y = 0.0f;
            world->velocities[entityIndex].z = 0.0f;
        }
    }
}
