/**
 * \file
 * \author Blake Caldwell
 * \brief Defines the shapes used in the intersection functions.
 */
#pragma once
#include "../math/vec3.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 *
 * \struct Sphere
 * \brief Structure to hold the data for a sphere.
 */
typedef struct
{
    float radius; /**< \brief The radius of the sphere. */
} Sphere;

/**
 * \struct AABB
 * \brief Structure to hold the data for an axis-aligned bounding box.
 */
typedef struct
{
    ac_vec3 half_extents; /**< \brief The half extents of the bounding box. */
} AABB;

#ifdef __cplusplus
}
#endif
