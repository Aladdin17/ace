#include "ace/physics/phys_world.h"
#include "ace/physics/phys_collision.h"
#include "memory.h"
#include "stdio.h"

void phys_init_world(PhysWorld* world)
{
    world->accumulator        = 0.0f;
    world->airResistance      = 0.3f;
    world->gravity            = (ac_vec3){ 0.0f, -9.8f, 0.0f };  // default gravity (9.8f
    world->numColliders       = 0;
    world->numDynamicEntities = 0;
    world->numEnts            = 0;
    world->numStaticEntities  = 0;
    world->timeStep           = 1.0f / 120.0f;
    world->velocityThreshhold = 0.075f;

    for ( unsigned i = 0; i < AC_MAX_PHYS_ENTS; i++ )
    {
        world->velocities[i] = ac_vec3_zero();  // default velocity (0.0f)
    }

    for ( unsigned i = 0; i < AC_MAX_PHYS_ENTS; i++ )
    {
        world->masses[i] = 1.0f;  // default mass (1.0f)
    }

    memset(world->positions, 0, sizeof(void*) * AC_MAX_PHYS_ENTS);
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
        world->positions[world->numEnts]  = *position;
        world->velocities[world->numEnts] = ac_vec3_zero();  // default velocity (0.0f)
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
        if ( world->sleeping[entity1] )
        {
            continue;
        }

        // check collisions between dynamic colliders
        for ( unsigned j = i + 1; j < world->numDynamicEntities; j++ )
        {
            entity2 = world->dynamicEntities[j];
            if ( world->sleeping[entity2] )
            {
                continue;
            }

            result = check_collision(
                &world->colliders[entity1],
                &world->positions[entity1],
                &world->colliders[entity2],
                &world->positions[entity2]
            );
            if ( result.intersected )
            {
                resolve_collision(
                    &result,
                    &world->positions[entity1],
                    &world->velocities[entity1],
                    world->masses[entity1],
                    false,
                    &world->positions[entity2],
                    &world->velocities[entity2],
                    world->masses[entity2],
                    false
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
            if ( world->sleeping[entity2] )
            {
                continue;
            }

            result = check_collision(
                &world->colliders[entity1],
                &world->positions[entity1],
                &world->colliders[entity2],
                &world->positions[entity2]
            );
            if ( result.intersected )
            {
                resolve_collision(
                    &result,
                    &world->positions[entity1],
                    &world->velocities[entity1],
                    world->masses[entity1],
                    false,
                    &world->positions[entity2],
                    &world->velocities[entity2],
                    world->masses[entity2],
                    true
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
{
    // semi implicit euler
    for ( unsigned i = 0; i < world->numDynamicEntities; i++ )
    {
        unsigned entityIndex = world->dynamicEntities[i];

        if ( world->sleeping[entityIndex] )
        {
            continue;
        }

        ac_vec3 delta_velocity = ac_vec3_scale(&world->gravity, world->timeStep);
        world->velocities[entityIndex] =
            ac_vec3_add(&world->velocities[entityIndex], &delta_velocity);
        world->velocities[entityIndex] = ac_vec3_scale(
            &world->velocities[entityIndex],
            1.0f - (world->airResistance * world->timeStep)
        );
        ac_vec3 delta_position = ac_vec3_scale(&world->velocities[entityIndex], world->timeStep);
        world->positions[entityIndex] =
            ac_vec3_add(&world->positions[entityIndex], &delta_position);

        //  if under the speed threshold, set velocity to 0
        if ( ac_vec3_magnitude(&world->velocities[entityIndex]) < world->velocityThreshhold )
        {
            world->velocities[entityIndex] = ac_vec3_zero();
        }
    }
}
