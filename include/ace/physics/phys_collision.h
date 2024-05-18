/**
 * \file phys_collision.h
 * \brief Contains the definitions for collision detection and resolution.
 */
#pragma once

#include "geometry/intersection.h"
#include "phys_components.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Checks for collision between two colliders.
 * \param c1 The first collider.
 * \param p1 The position of the first collider.
 * \param c2 The second collider.
 * \param p2 The position of the second collider.
 * \return The result of the collision check.
 */
IntersectionResult check_collision(
    Collider* c1, ac_vec3* const p1, Collider* c2, ac_vec3* const p2
);

/**
 * \brief Resolves a collision between two objects.
 * \param info The result of the collision check.
 * \param pos1 The position of the first object.
 * \param v1 The velocity of the first object.
 * \param m1 The mass of the first object.
 * \param s1 The static flag of the first object.
 * \param pos2 The position of the second object.
 * \param v2 The velocity of the second object.
 * \param m2 The mass of the second object.
 * \param s2 The static flag of the second object.
 */
void resolve_collision(
    IntersectionResult* info,
    ac_vec3*            pos1,
    ac_vec3*            v1,
    float               m1,
    bool                s1,
    ac_vec3*            pos2,
    ac_vec3*            v2,
    float               m2,
    bool                s2
);

#ifdef __cplusplus
}
#endif
