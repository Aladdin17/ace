/**
 * \file phys_components.h
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
    SPHERE, /**< \brief Sphere collider type. */
    AABB,   /**< \brief Axis-aligned bounding box collider type. */
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
typedef void (*PhysCallBack)(void);

#ifdef __cplusplus
}
#endif
