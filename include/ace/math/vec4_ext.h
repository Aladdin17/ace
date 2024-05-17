/**
 * \file
 * \author Christien Alden
 * \brief Provides additional functions for 4-component vectors.
 * \details
 * This file provides additional functions for 4w-component vectors that are not
 * considered 'core' functions. These functions are not required for the basic
 * functionality of a 4-component vector, but are useful for more advanced
 * operations. Notably these functions can be derived from the core functions,
 * but are provided here for convenience and to avoid code duplication.
 */
#pragma once
#include "mat4.h"
#include "vec4.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup vec4
 * \brief Calculate the angle between two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The angle between the two vectors in radians.
 * \details
 * If either vector is of zero length or NaN, the returned angle will be NaN.
 */
float   ac_vec4_angle(const ac_vec4* a, const ac_vec4* b);
/**
 * \ingroup vec4
 * \brief Calculate the Euclidean distance between two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The distance between the two vectors.
 * \details
 * If either vector is of NaN, the returned distance will be NaN.
 */
float   ac_vec4_distance(const ac_vec4* a, const ac_vec4* b);
/**
 * \ingroup vec4
 * \brief Linearly interpolate between two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \param[in] delta The interpolation factor.
 * \return The interpolated vector.
 */
ac_vec4 ac_vec4_lerp(const ac_vec4* a, const ac_vec4* b, float interpolation_factor);
/**
 * \ingroup vec4
 * \brief Project a vector onto another vector.
 * \param[in] a The vector to project.
 * \param[in] b The vector to project onto.
 * \return The projected vector, a onto b.
 * \details
 * If either vector is of zero length or NaN, the returned vector will have NaN components.
 */
ac_vec4 ac_vec4_project(const ac_vec4* a, const ac_vec4* b);
/**
 * \ingroup vec4
 * \brief Calculate the reflected vector about a normal.
 * \param[in] v The vector to reflect.
 * \param[in] n The normal vector.
 * \return The reflected vector.
 * \details
 * If either vector is of zero length or NaN, the returned vector will have NaN components.
 */
ac_vec4 ac_vec4_reflect(const ac_vec4* incoming, const ac_vec4* normal);
/**
 * \ingroup vec4
 * \brief Calculates a vector that has been multiplied by a matrix.
 * \param[in] vec The vector to multiply.
 * \param[in] mat The matrix to multiply by.
 * \return The resulting vector.
 * \details
 * If either \p vec or \p mat fails their respective is_nan check, the resulting
 * vector will be the nan vector.
 */
ac_vec4 ac_vec4_mult_matrix(const ac_vec4* vec, const ac_mat4* mat);

#ifdef __cplusplus
}
#endif
