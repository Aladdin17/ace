/**
 * \file
 * \author Christien Alden
 * \brief 3-component vector types and functions.
 */
#pragma once
#include <limits.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup vec3 vec3
 * \brief 3-component float vector and functions.
 */
/**
 * \defgroup ivec3 ivec3
 * \brief 3-component integer vector and functions.
 */

//--------------------------------------------------------------------------------------------------
// float
//--------------------------------------------------------------------------------------------------

/**
 * \ingroup vec3
 * \union ac_vec3
 * \brief A 3-component vector of type float.
 * \details
 * A standard 3-component vector of type float which provides 'aliases' for
 * axes, colors, and elements.
 */
typedef union ac_vec3
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
} ac_vec3;

/**
 * \ingroup vec3
 * \brief Creates a vector with all components set to zero.
 * \return A vector with all components set to zero.
 */
ac_vec3 ac_vec3_zero(void);
/**
 * \ingroup vec3
 * \brief Creates a vector with all components set to NaN.
 * \return A vector with all components set to NaN.
 */
ac_vec3 ac_vec3_nan(void);
/**
 * \ingroup vec3
 * \brief Checks if a vector has zero components.
 * \param[in] v The vector.
 * \retval true if all of the components of the vector are zero, false otherwise.
 * \retval false one or more of the components of the vector are non-zero.
 * \note This function uses \ref AC_EPSILON as the epsilon value for floating point comparisons.
 */
bool    ac_vec3_is_zero(const ac_vec3* v);
/**
 * \ingroup vec3
 * \brief Checks if a vector has NaN components.
 * \param[in] v The vector.
 * \return True if any of the components of the vector are NaN, false otherwise.
 */
bool    ac_vec3_is_nan(const ac_vec3* v);
/**
 * \ingroup vec3
 * \brief Checks if two vectors are equal.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \retval true if all of the components of the vectors are equal, false otherwise.
 * \retval false if one or more of the components of the vectors are not equal.
 * \retval false if either vector has NaN components.
 * \note This function uses \ref AC_EPSILON as the epsilon value for floating point comparisons.
 */
bool    ac_vec3_is_equal(const ac_vec3* a, const ac_vec3* b);
/**
 * \ingroup vec3
 * \brief Adds two vectors: a + b.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The sum of the two vectors.
 */
ac_vec3 ac_vec3_add(const ac_vec3* a, const ac_vec3* b);
/**
 * \ingroup vec3
 * \brief Subtracts two vectors: a - b.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The difference of the two vectors.
 */
ac_vec3 ac_vec3_sub(const ac_vec3* a, const ac_vec3* b);
/**
 * \ingroup vec3
 * \brief Negates a vector.
 * \param[in] v The vector to negate.
 * \return The negated vector.
 */
ac_vec3 ac_vec3_negate(const ac_vec3* v);
/**
 * \ingroup vec3
 * \brief Multiplies a vector by a scalar: a * scalar.
 * \param[in] v The vector.
 * \param[in] scalar The scalar.
 * \return The scaled vector.
 */
ac_vec3 ac_vec3_scale(const ac_vec3* v, float scalar);
/**
 * \ingroup vec3
 * \brief Computes the dot product of two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The dot product of the two vectors.
 */
float   ac_vec3_dot(const ac_vec3* a, const ac_vec3* b);
/**
 * \ingroup vec3
 * \brief Computes the cross product of two vectors: a x b.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The cross product of the two vectors.
 */
ac_vec3 ac_vec3_cross(const ac_vec3* a, const ac_vec3* b);
/**
 * \ingroup vec3
 * \brief Computes the length of a vector.
 * \param[in] v The vector.
 * \return The length of the vector.
 */
float   ac_vec3_magnitude(const ac_vec3* v);
/**
 * \ingroup vec3
 * \brief Normalizes a vector.
 * \param[in] v The vector.
 * \return The normalized vector.
 * \details
 * If the vector is of zero length or NaN, the returned vector will have NaN components.
 */
ac_vec3 ac_vec3_normalize(const ac_vec3* v);

//--------------------------------------------------------------------------------------------------
// int
//--------------------------------------------------------------------------------------------------

/**
 * \ingroup ivec3
 * \def INT_INVALID
 * \brief A sentinel value for an invalid integer.
 */
#define INT_INVALID INT_MIN

/**
 * \ingroup ivec3
 * \union ivec3
 * \brief A 3-component vector of type int.
 * \details
 * A standard 3-component vector of type int which provides 'aliases' for
 * axes, colors, and elements.
 */
typedef union ac_ivec3
{
    struct
    {
        int x, y, z;
    };
    struct
    {
        int r, g, b;
    };
    int data[3];
} ac_ivec3;

/**
 * \ingroup ivec3
 * \brief Creates a vector with all components set to zero.
 * \return A vector with all components set to zero.
 */
ac_ivec3 ac_ivec3_zero(void);
/**
 * \ingroup ivec3
 * \brief Creates a vector with all components set to an invalid sentinel.
 * \return A vector with all components set to an invalid sentinel.
 * \see INT_INVALID
 */
ac_ivec3 ac_ivec3_invalid(void);
/**
 * \ingroup ivec3
 * \brief Checks if a vector has zero components.
 * \param[in] v The vector.
 * \retval true if all of the components of the vector are zero.
 * \retval false one or more of the components of the vector are non-zero.
 */
bool     ac_ivec3_is_zero(const ac_ivec3* v);
/**
 * \ingroup ivec3
 * \brief Checks if a vector has invalid components.
 * \param[in] v The vector.
 * \retval true if any of the components of the vector are invalid.
 * \retval false if all of the components of the vector are valid.
 * \see INT_INVALID
 */
bool     ac_ivec3_is_invalid(const ac_ivec3* v);
/**
 * \ingroup ivec3
 * \brief Checks if two vectors are equal.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \retval true if all of the components of the vectors are equal.
 * \retval false if one or more of the components of the vectors are not equal.
 */
bool     ac_ivec3_is_equal(const ac_ivec3* a, const ac_ivec3* b);
/**
 * \ingroup ivec3
 * \brief Adds two vectors: a + b.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The sum of the two vectors.
 * \note If either vector has invalid components, the result will be the invalid ac_ivec3.
 * \see ac_ivec3_is_invalid
 */
ac_ivec3 ac_ivec3_add(const ac_ivec3* a, const ac_ivec3* b);
/**
 * \ingroup ivec3
 * \brief Subtracts two vectors: a - b.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The difference of the two vectors.
 * \note If either vector has invalid components, the result will be the invalid ac_ivec3.
 * \see ac_ivec3_is_invalid
 */
ac_ivec3 ac_ivec3_sub(const ac_ivec3* a, const ac_ivec3* b);
/**
 * \ingroup ivec3
 * \brief Negates a vector.
 * \param[in] v The vector to negate.
 * \return The negated vector.
 * \note If the vector has invalid components, the result will be the invalid ac_ivec3.
 * \see ac_ivec3_is_invalid
 */
ac_ivec3 ac_ivec3_negate(const ac_ivec3* v);
/**
 * \ingroup ivec3
 * \brief Multiplies a vector by a scalar: a * scalar.
 * \param[in] v The vector.
 * \param[in] scalar The scalar.
 * \return The scaled vector.
 * \note If the vector has invalid components, the result will be the invalid ac_ivec3.
 * \see ac_ivec3_is_invalid
 */
ac_ivec3 ac_ivec3_scale(const ac_ivec3* v, int scalar);
/**
 * \ingroup ivec3
 * \brief Divides a vector by a scalar: a / scalar.
 * \param[in] v The vector.
 * \param[in] scalar The scalar.
 * \return The scaled vector.
 * \note If the vector has invalid components, the result will be the invalid ac_ivec3.
 * \note If the scalar is zero, the result will be the invalid ac_ivec3.
 * \see ac_ivec3_is_invalid
 * \details
 * This function uses integer division, so the result will be truncated.
 * The result of this integer division is defined by the C standard to be the
 * floor of the division. If you need a different behavior for this rounding,
 * use \ref ac_ivec3_divide_ext.
 */
ac_ivec3 ac_ivec3_divide(const ac_ivec3* v, int scalar);
/**
 * \ingroup ivec3
 * \brief Divides a vector by a scalar: a / scalar.
 * \param[in] v The vector.
 * \param[in] scalar The scalar.
 * \param[in] rounding_func Used to round the result of the division.
 * \return The scaled vector.
 * \note If the vector has invalid components, the result will be the invalid ac_ivec3.
 * \note If the scalar is zero, the result will be the invalid ac_ivec3.
 * \see ac_ivec3_is_invalid
 * \details
 * This function uses a rounding function to determine the result of the division.
 * The rounding function should take a float as input and return an integer.
 * The result of the division is then rounded using this function.
 * If the result of the standard C integer division is desired, use \ref ac_ivec3_divide.
 */
ac_ivec3 ac_ivec3_divide_ext(const ac_ivec3* v, int scalar, int (*rounding_func)(float));
/**
 * \ingroup ivec3
 * \brief Computes the dot product of two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The dot product of the two vectors.
 * \note If either vector has invalid components, the result will be \ref INT_INVALID.
 */
int      ac_ivec3_dot(const ac_ivec3* a, const ac_ivec3* b);
/**
 * \ingroup ivec3
 * \brief Computes the cross product of two vectors: a x b.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The cross product of the two vectors.
 * \note If either vector has invalid components, the result will be the invalid ac_ivec3.
 * \see ac_ivec3_is_invalid
 */
ac_ivec3 ac_ivec3_cross(const ac_ivec3* a, const ac_ivec3* b);

#ifdef __cplusplus
}
#endif
