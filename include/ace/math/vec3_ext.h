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
#include "mat3.h"
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
float   ac_vec3_distance(const ac_vec3* a, const ac_vec3* b);
/**
 * \ingroup vec3
 * \brief Linearly interpolate between two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \param[in] interpolation_factor The amount to interpolate between the two vectors.
 * \return The interpolated vector.
 */
ac_vec3 ac_vec3_lerp(const ac_vec3* a, const ac_vec3* b, float interpolation_factor);
/**
 * \ingroup vec3
 * \brief Project a vector onto another vector.
 * \param[in] a The vector to project.
 * \param[in] b The vector to project onto.
 * \return The projected vector, a onto b.
 * \details
 * If either vector is of zero length or NaN, the returned vector will have NaN components.
 */
ac_vec3 ac_vec3_project(const ac_vec3* a, const ac_vec3* b);
/**
 * \ingroup vec3
 * \brief Calculate the reflected vector about a normal.
 * \param[in] incoming The vector to reflect.
 * \param[in] normal The normal vector.
 * \return The reflected vector.
 * \details
 * If either vector is of zero length or NaN, the returned vector will have NaN components.
 */
ac_vec3 ac_vec3_reflect(const ac_vec3* incoming, const ac_vec3* normal);
/**
 * \ingroup vec3
 * \brief Calculates a vector that has been multiplied by a matrix.
 * \param[in] vec The vector to multiply.
 * \param[in] mat The matrix to multiply by.
 * \return The resulting vector.
 * \details
 * If either \p vec or \p mat fails their respective is_nan check, the resulting
 * vector will be the nan vector.
 */
ac_vec3 ac_vec3_mult_matrix(const ac_vec3* vec, const ac_mat3* mat);

#ifdef __cplusplus
}
#endif
