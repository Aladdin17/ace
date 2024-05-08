#include "phys_world.h"

#include "memory.h"

#include "stdio.h"

#include "phys_collision.h"

void phys_init_world(PhysWorld *world) {
  world->numEnts = world->numColliders = world->numDynamicEntities =
      world->numStaticEntities = 0;

  world->gravity.x = 0.0f;
  world->gravity.z = 0.0f;
  world->gravity.y = -9.8f;
  world->airResistance = 0.1f;

  world->timeStep = 1.0f / 500.0f;

  memset(world->positions, 0, sizeof(void *) * AC_MAX_PHYS_ENTS);
  memset(world->velocities, 0, sizeof(ac_vec3) * AC_MAX_PHYS_ENTS);

  memset(world->masses, 1, sizeof(float) * AC_MAX_PHYS_ENTS);

  memset(world->configs, 0, sizeof(PhysConfig) * AC_MAX_PHYS_ENTS);
  memset(world->callbacks, 0, sizeof(void *) * AC_MAX_PHYS_ENTS);

  memset(world->colliders, 0, sizeof(Collider) * AC_MAX_PHYS_ENTS);

  memset(world->staticEntities, 0, sizeof(unsigned) * AC_MAX_PHYS_ENTS);
  memset(world->dynamicEntities, 0, sizeof(unsigned) * AC_MAX_PHYS_ENTS);
}

unsigned phys_add_entity(PhysWorld *world, ac_vec3 *position) {
  if (world->numEnts < AC_MAX_PHYS_ENTS) {
    world->positions[world->numEnts] = position;

    world->velocities[world->numEnts].x = 0.0f;
    world->velocities[world->numEnts].y = 0.0f;
    world->velocities[world->numEnts].z = 0.0f;

    world->numEnts++; // not the best - if in future i have time [fix]
    return world->numEnts - 1;
  }
  return AC_PHYS_ERROR_ENT;
}

void phys_add_entity_collider(PhysWorld *world, Collider collider,
                              unsigned entity) {
  if (world->numColliders <= world->numEnts && entity <= world->numEnts) {
    world->colliders[entity] = collider;
    world->numColliders++;
  }
}

// NEED TO MAKE THEES MORE ROBUST

void phys_make_entity_dynamic(PhysWorld *world, unsigned entity) {
  world->dynamicEntities[world->numDynamicEntities] = entity;
  world->numDynamicEntities++;
}

void phys_make_entity_static(PhysWorld *world, unsigned entity) {
  world->staticEntities[world->numStaticEntities] = entity;
  world->numStaticEntities++;
}

// update funcs

void update_collisions(PhysWorld *world);
void update_movements(PhysWorld *world);

void phys_update(PhysWorld *world, float deltaTime) {
  world->accumulator += deltaTime;

  int i = 0;

  while (world->accumulator >= world->timeStep) {
    update_movements(world);
    update_collisions(world);
    world->accumulator -= world->timeStep;
    i++;
  }
}

void update_collisions(PhysWorld *world) {
  CollisionResult result;

  unsigned entity1 = 0, entity2 = 0;
  for (unsigned i = 0; i < world->numDynamicEntities; i++) {
    entity1 = world->dynamicEntities[i];

    // check collisions between dynamic colliders
    for (unsigned j = i + 1; j < world->numDynamicEntities; j++) {
      entity2 = world->dynamicEntities[j];

      result = check_collision(
          &world->colliders[entity1], world->positions[entity1],
          &world->colliders[entity2], world->positions[entity2]);
      if (result.collided)
        resolve_collision(
            &result, world->positions[entity1], &world->velocities[entity1],
            world->masses[entity1], 0, world->positions[entity2],
            &world->velocities[entity2], world->masses[entity2], 0);
    }

    // check collisions between dynamic and static colliders
    for (unsigned j = 0; j < world->numStaticEntities; j++) {
      entity2 = world->staticEntities[j];

      result = check_collision(
          &world->colliders[entity1], world->positions[entity1],
          &world->colliders[entity2], world->positions[entity2]);
      if (result.collided)
        resolve_collision(
            &result, world->positions[entity1], &world->velocities[entity1],
            world->masses[entity1], 0, world->positions[entity2],
            &world->velocities[entity2], world->masses[entity2], 1);
    }
  }
}

void update_movements(PhysWorld *world) // might make this verlet if I have time
{
  for (unsigned i = 0; i < world->numDynamicEntities; i++) {

    // apply gravity
    world->velocities[world->dynamicEntities[i]].x +=
        world->gravity.x * world->timeStep;
    world->velocities[world->dynamicEntities[i]].y +=
        world->gravity.y * world->timeStep;
    world->velocities[world->dynamicEntities[i]].z +=
        world->gravity.z * world->timeStep;

    // apply air resistance
    world->velocities[world->dynamicEntities[i]].x *=
        1.0f - (world->airResistance * world->timeStep);
    world->velocities[world->dynamicEntities[i]].y *=
        1.0f - (world->airResistance * world->timeStep);
    world->velocities[world->dynamicEntities[i]].z *=
        1.0f - (world->airResistance * world->timeStep);

    // update positions
    world->positions[world->dynamicEntities[i]]->x +=
        world->velocities[world->dynamicEntities[i]].x * world->timeStep;
    world->positions[world->dynamicEntities[i]]->y +=
        world->velocities[world->dynamicEntities[i]].y * world->timeStep;
    world->positions[world->dynamicEntities[i]]->z +=
        world->velocities[world->dynamicEntities[i]].z * world->timeStep;
  }
}