/**
 * \file phys_primitives.h
 * \brief Contains the definitions for physics collision primitives.
 */
#pragma once

#include "math/vec3.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \struct SphereData
 * \brief Structure to hold the data for a sphere.
 */
typedef struct {
  float radius; /**< \brief The radius of the sphere. */
} SphereData;

/**
 * \struct AABBData
 * \brief Structure to hold the data for an axis-aligned bounding box.
 */
typedef struct {
  ac_vec3 half_extents; /**< \brief The half extents of the bounding box. */
} AABBData;

/**
 * \enum ColliderType
 * \brief Enumeration for the types of colliders.
 */
enum ColliderType {
  SPHERE, /**< \brief Sphere collider type. */
  AABB,   /**< \brief Axis-aligned bounding box collider type. */
};

#ifdef __cplusplus
}
#endif
