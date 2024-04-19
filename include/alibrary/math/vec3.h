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
 * \union Vec3
 * \brief A 3-component vector of type float.
 * \details
 * A standard 3-component vector of type float which provides 'aliases' for
 * axes, colors, and elements.
 */
typedef union Vec3
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
} Vec3;

/**
 * \brief Creates a vector with all components set to zero.
 * \return A vector with all components set to zero.
 */
Vec3  vec3_zero(void);
/**
 * \brief Creates a vector with all components set to NaN.
 * \return A vector with all components set to NaN.
 */
Vec3  vec3_nan(void);
/**
 * \brief Checks if a vector has zero components.
 * \param[in] v The vector.
 * \retval true if all of the components of the vector are zero, false otherwise.
 * \retval false one or more of the components of the vector are non-zero.
 * \note This function uses AL_EPSILON as the epsilon value for floating point comparisons.
 */
bool  vec3_is_zero(const Vec3* v);
/**
 * \brief Checks if a vector has NaN components.
 * \param[in] v The vector.
 * \return True if any of the components of the vector are NaN, false otherwise.
 */
bool  vec3_is_nan(const Vec3* v);
/**
 * \brief Adds two vectors: a + b.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The sum of the two vectors.
 */
Vec3  vec3_add(const Vec3* a, const Vec3* b);
/**
 * \brief Subtracts two vectors: a - b.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The difference of the two vectors.
 */
Vec3  vec3_sub(const Vec3* a, const Vec3* b);
/**
 * \brief Negates a vector.
 * \param[in] v The vector to negate.
 * \return The negated vector.
 */
Vec3  vec3_negate(const Vec3* v);
/**
 * \brief Multiplies a vector by a scalar: a * scalar.
 * \param[in] v The vector.
 * \param[in] scalar The scalar.
 * \return The scaled vector.
 */
Vec3  vec3_scale(const Vec3* v, float scalar);
/**
 * \brief Computes the dot product of two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The dot product of the two vectors.
 */
float vec3_dot(const Vec3* a, const Vec3* b);
/**
 * \brief Computes the cross product of two vectors: a x b.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The cross product of the two vectors.
 */
Vec3  vec3_cross(const Vec3* a, const Vec3* b);
/**
 * \brief Computes the length of a vector.
 * \param[in] v The vector.
 * \return The length of the vector.
 */
float vec3_magnitude(const Vec3* v);
/**
 * \brief Normalizes a vector.
 * \param[in] v The vector.
 * \return The normalized vector.
 * \note If the length of the vector is 0, the components of the returned vector are NaN.
 * \see Vec3IsNaN
 */
Vec3  vec3_normalize(const Vec3* v);

//--------------------------------------------------------------------------------------------------
// Utility functions
//--------------------------------------------------------------------------------------------------

/**
 * \brief Calculate the angle between two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The angle between the two vectors in radians.
 */
float vec3_angle(const Vec3* a, const Vec3* b);
/**
 * \brief Calculate the reflected vector about a normal.
 * \param[in] v The vector to reflect.
 * \param[in] n The normal vector.
 * \return The reflected vector.
 * \note The returned vector will be NaN if either the vector or normal are zero or nan vectors.
 */
Vec3  vec3_reflect(const Vec3* incoming, const Vec3* normal);

#ifdef __cplusplus
}
#endif
