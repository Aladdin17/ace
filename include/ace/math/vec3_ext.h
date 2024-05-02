/**
 * \file
 * \author Christien Alden
 * \brief Provides additional functions for 3-component vectors.
 * \details
 * This file provides additional functions for 3-component vectors that are not
 * considered 'core' functions. These functions are not required for the basic
 * functionality of a 3-component vector, but are useful for more advanced
 * operations. Notably these functions can be derived from the core functions,
 * but are provided here for convenience and to avoid code duplication.
 */
#pragma once
#include "vec3.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup vec3
 * \brief Calculate the angle between two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The angle between the two vectors in radians.
 * \details
 * If either vector is of zero length or NaN, the returned angle will be NaN.
 */
float   ac_vec3_angle(const ac_vec3* a, const ac_vec3* b);
/**
 * \ingroup vec3
 * \brief Calculate the Euclidean distance between two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The distance between the two vectors.
 * \details
 * If either vector is of NaN, the returned distance will be NaN.
 */
float   vec3_distance(const ac_vec3* a, const ac_vec3* b);
/**
 * \ingroup vec3
 * \brief Linearly interpolate between two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \param[in] delta The interpolation factor.
 * \return The interpolated vector.
 */
ac_vec3 vec3_lerp(const ac_vec3* a, const ac_vec3* b, float interpolation_factor);
/**
 * \ingroup vec3
 * \brief Project a vector onto another vector.
 * \param[in] a The vector to project.
 * \param[in] b The vector to project onto.
 * \return The projected vector, a onto b.
 * \details
 * If either vector is of zero length or NaN, the returned vector will have NaN components.
 */
ac_vec3 vec3_project(const ac_vec3* a, const ac_vec3* b);
/**
 * \ingroup vec3
 * \brief Calculate the reflected vector about a normal.
 * \param[in] v The vector to reflect.
 * \param[in] n The normal vector.
 * \return The reflected vector.
 * \details
 * If either vector is of zero length or NaN, the returned vector will have NaN components.
 */
ac_vec3 vec3_reflect(const ac_vec3* incoming, const ac_vec3* normal);

#ifdef __cplusplus
}
#endif
