/**
 * \file phys_components.h
 * \brief Contains the definitions for physics components.
 */
#pragma once

#include "phys_primitives.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \struct Collider
 * \brief Structure to hold the collider data.
 */
typedef struct {
  enum ColliderType type; /**< \brief The type of the collider. */
  void *data;             /**< \brief The data of the collider. */
} Collider;

/**
 * \struct PhysConfig
 * \brief Structure to hold the configuration for a physical object.
 */
typedef struct {
  unsigned int flags; /**< \brief Flags to configure the behavior of the
                         physical object. */
} PhysConfig;

/**
 * \typedef PhysCallBack
 * \brief Typedef for a callback function.
 */
typedef void (*PhysCallBack)(int);

#ifdef __cplusplus
}
#endif
