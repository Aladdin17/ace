/**
 * \file
 * \author Blake Caldwell
 * \brief Contains the definitions for physics components.
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \enum ColliderType
 * \brief Enumeration for the types of colliders.
 */
enum ColliderType
{
    SPHERE_C, /**< \brief Sphere collider type. */
    AABB_C,   /**< \brief Axis-aligned bounding box collider type. */
};

/**
 * \struct Collider
 * \brief Structure to hold the collider data.
 */
typedef struct
{
    enum ColliderType type; /**< \brief The type of the collider. */
    void*             data; /**< \brief The data of the collider. */
} Collider;

/**
 * \typedef PhysCallBack
 * \brief Typedef for a callback function.
 */
typedef void (*PhysCallBack)(unsigned, unsigned);

#ifdef __cplusplus
}
#endif
