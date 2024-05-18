#pragma once
#include "../math/vec3.h"
#include "shapes.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Structure to hold the result of a collision check.
 */
typedef struct
{
    bool    intersected;      /**< \brief Flag indicating if objects are intersecting. */
    ac_vec3 contactNormal;    /**< \brief The normal vector at the point of contact. */
    float   penetrationDepth; /**< \brief The depth of penetration of intersection. */
    ac_vec3 contactPoint;     /**< \brief The point of contact during the intersection. */

} IntersectionResult;

/**
 * \brief Check for intersection between two spheres.
 *
 * \param sphere1 The first sphere.
 * \param p1 Pointer to the position of the first sphere.
 * \param sphere2 The second sphere.
 * \param p2 Pointer to the position of the second sphere.
 *
 * \return IntersectionResult structure containing information about the
 * intersection.
 */
IntersectionResult sphere_sphere(
    Sphere sphere1, const ac_vec3* const p1, Sphere sphere2, const ac_vec3* const p2
);

/**
 * \brief Check for intersection between a sphere and an AABB (Axis-Aligned
 * Bounding Box).
 *
 * \param sphere The sphere.
 * \param p1 Pointer to the position of the sphere.
 * \param aabbHalfExtents Pointer to the AABB structure representing the
 * half-extents of the AABB. \param p2 Pointer to the position of the AABB.
 *
 * \return IntersectionResult structure containing information about the
 * intersection.
 */
IntersectionResult sphere_AABB(
    Sphere sphere, const ac_vec3* const p1, AABB* aabbHalfExtents, const ac_vec3* const p2
);

#ifdef __cplusplus
}
#endif
