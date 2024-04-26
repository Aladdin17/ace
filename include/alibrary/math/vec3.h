/**
 * \file
 * \author Christien Alden
 * \brief 3-component vector types and functions.
 */
#pragma once
#include "math.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \union vec3
 * \brief A 3-component vector of type float.
 * \details
 * A standard 3-component vector of type float which provides 'aliases' for
 * axes, colors, and elements.
 */
typedef union vec3
{
    struct
    {
        float x, y, z;
    };
    struct
    {
        float r, g, b;
    };
    float data[3];
} vec3;

/**
 * \brief Creates a vector with all components set to zero.
 * \return A vector with all components set to zero.
 */
vec3  vec3_zero(void);
/**
 * \brief Creates a vector with all components set to NaN.
 * \return A vector with all components set to NaN.
 */
vec3  vec3_nan(void);
/**
 * \brief Checks if a vector has zero components.
 * \param[in] v The vector.
 * \retval true if all of the components of the vector are zero, false otherwise.
 * \retval false one or more of the components of the vector are non-zero.
 * \note This function uses \ref AL_EPSILON as the epsilon value for floating point comparisons.
 */
bool  vec3_is_zero(const vec3* v);
/**
 * \brief Checks if a vector has NaN components.
 * \param[in] v The vector.
 * \return True if any of the components of the vector are NaN, false otherwise.
 */
bool  vec3_is_nan(const vec3* v);
/**
 * \brief Checks if two vectors are equal.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \retval true if all of the components of the vectors are equal, false otherwise.
 * \retval false if one or more of the components of the vectors are not equal.
 * \retval false if either vector has NaN components.
 * \note This function uses \ref AL_EPSILON as the epsilon value for floating point comparisons.
 */
bool  vec3_is_equal(const vec3* a, const vec3* b);
/**
 * \brief Adds two vectors: a + b.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The sum of the two vectors.
 */
vec3  vec3_add(const vec3* a, const vec3* b);
/**
 * \brief Subtracts two vectors: a - b.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The difference of the two vectors.
 */
vec3  vec3_sub(const vec3* a, const vec3* b);
/**
 * \brief Negates a vector.
 * \param[in] v The vector to negate.
 * \return The negated vector.
 */
vec3  vec3_negate(const vec3* v);
/**
 * \brief Multiplies a vector by a scalar: a * scalar.
 * \param[in] v The vector.
 * \param[in] scalar The scalar.
 * \return The scaled vector.
 */
vec3  vec3_scale(const vec3* v, float scalar);
/**
 * \brief Computes the dot product of two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The dot product of the two vectors.
 */
float vec3_dot(const vec3* a, const vec3* b);
/**
 * \brief Computes the cross product of two vectors: a x b.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The cross product of the two vectors.
 */
vec3  vec3_cross(const vec3* a, const vec3* b);
/**
 * \brief Computes the length of a vector.
 * \param[in] v The vector.
 * \return The length of the vector.
 */
float vec3_magnitude(const vec3* v);
/**
 * \brief Normalizes a vector.
 * \param[in] v The vector.
 * \return The normalized vector.
 * \details
 * If the vector is of zero length or NaN, the returned vector will have NaN components.
 */
vec3  vec3_normalize(const vec3* v);

//--------------------------------------------------------------------------------------------------
// Utility functions
//--------------------------------------------------------------------------------------------------

/**
 * \brief Calculate the angle between two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The angle between the two vectors in radians.
 * \details
 * If either vector is of zero length or NaN, the returned angle will be NaN.
 */
float vec3_angle(const vec3* a, const vec3* b);
/**
 * \brief Calculate the reflected vector about a normal.
 * \param[in] v The vector to reflect.
 * \param[in] n The normal vector.
 * \return The reflected vector.
 * \details
 * If either vector is of zero length or NaN, the returned vector will have NaN components.
 */
vec3  vec3_reflect(const vec3* incoming, const vec3* normal);
/**
 * \brief Calculate the Euclidean distance between two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The distance between the two vectors.
 * \details
 * If either vector is of NaN, the returned distance will be NaN.
 */
float vec3_distance(const vec3* a, const vec3* b);
/**
 * \brief Linearly interpolate between two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \param[in] delta The interpolation factor.
 * \return The interpolated vector.
 */
vec3  vec3_lerp(const vec3* a, const vec3* b, float interpolation_factor);
/**
 * \brief Project a vector onto another vector.
 * \param[in] a The vector to project.
 * \param[in] b The vector to project onto.
 * \return The projected vector, a onto b.
 * \details
 * If either vector is of zero length or NaN, the returned vector will have NaN components.
 */
vec3  vec3_project(const vec3* a, const vec3* b);

#ifdef __cplusplus
}
#endif
