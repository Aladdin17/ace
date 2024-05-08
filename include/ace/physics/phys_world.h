/**
 * \file phys_world.h
 * \brief Contains the definitions for the physics
 */
#pragma once

#include "phys_components.h"

#define AC_MAX_PHYS_ENTS 100 // should be vectors instead
#define AC_PHYS_ERROR_ENT 2147483646

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \struct PhysWorld
 * \brief Structure to hold the data for the physics world.
 */
typedef struct {
  ac_vec3 *
      positions[AC_MAX_PHYS_ENTS]; /**< \brief The positions of the entities. */
  ac_vec3 velocities[AC_MAX_PHYS_ENTS]; /**< \brief The velocities of the
                                           entities. */

  float masses[AC_MAX_PHYS_ENTS]; /**< \brief The masses of the entities. */

  PhysConfig configs[AC_MAX_PHYS_ENTS]; /**< \brief The configurations of the
                                           entities. */
  PhysCallBack callbacks[AC_MAX_PHYS_ENTS]; /**< \brief The on contact callbacks
                                               of the entities. */

  unsigned numEnts; /**< \brief The number of entities. */

  Collider
      colliders[AC_MAX_PHYS_ENTS]; /**< \brief The colliders of the entities. */
  unsigned numColliders;           /**< \brief The number of colliders. */

  unsigned
      staticEntities[AC_MAX_PHYS_ENTS]; /**< \brief The static entities ids. */
  unsigned dynamicEntities[AC_MAX_PHYS_ENTS]; /**< \brief The dynamic entities
                                                 ids. */
  unsigned numStaticEntities;  /**< \brief The number of static entities. */
  unsigned numDynamicEntities; /**< \brief The number of dynamic entities. */

  ac_vec3 gravity;     /**< \brief The gravity of the world. */
  float airResistance; /**< \brief The air resistance of the world. */

  float accumulator; /**< \brief The accumulator for the world. */
  float timeStep;    /**< \brief The time step for the world. */
} PhysWorld;

/**
 * \brief Initializes the physics world .
 * \param world The world to initialize.
 */
void phys_init_world(PhysWorld *world);

/**
 * \brief Adds an entity to the world.
 * \param world The world to add the entity to.
 * \param position The position of the entity.
 * \return The ID of the added entity.
 */
unsigned phys_add_entity(PhysWorld *world, ac_vec3 *position);

/**
 * \brief Adds a collider to an entity in the world.
 * \param world The world where the entity resides.
 * \param collider The collider to add to the entity.
 * \param entity The ID of the entity.
 */
void phys_add_entity_collider(PhysWorld *world, Collider collider,
                              unsigned entity);

/**
 * \brief Makes an entity dynamic.
 * \param world The world where the entity resides.
 * \param entity The ID of the entity.
 */
void phys_make_entity_dynamic(PhysWorld *world, unsigned entity);

/**
 * \brief Makes an entity static.
 * \param world The world where the entity resides.
 * \param entity The ID of the entity.
 */
void phys_make_entity_static(PhysWorld *world, unsigned entity);

/**
 * \brief Updates the physics world.
 * \param world The world to update.
 * \param deltaTime The time elapsed since the last update.
 */
void phys_update(PhysWorld *world, float deltaTime);

#ifdef __cplusplus
}
#endif
