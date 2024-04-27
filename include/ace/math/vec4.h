/**
 * \file
 * \author Christien Alden
 * \brief 4-component vector types and functions.
 */
#pragma once
#include <limits.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup vec4 vec4
 * \brief 4-component float vector and functions.
 */
/**
 * \defgroup ivec4 ivec4
 * \brief 4-component integer vector and functions.
 */

//--------------------------------------------------------------------------------------------------
// float
//--------------------------------------------------------------------------------------------------

/**
 * \ingroup vec4
 * \union ac_vec4
 * \brief A 4-component vector of type float.
 * \details
 * A standard 4-component vector of type float which provides 'aliases' for
 * axes, colors, and elements.
 */
typedef union ac_vec4
{
    struct
    {
        float x, y, z, w;
    };
    struct
    {
        float r, g, b, a;
    };
    float data[4];
} ac_vec4;

/**
 * \ingroup vec4
 * \brief Creates a vector with all components set to zero.
 * \return A vector with all components set to zero.
 */
ac_vec4 ac_vec4_zero(void);
/**
 * \ingroup vec4
 * \brief Creates a vector with all components set to NaN.
 * \return A vector with all components set to NaN.
 */
ac_vec4 ac_vec4_nan(void);
/**
 * \ingroup vec4
 * \brief Checks if a vector has zero components.
 * \param[in] v The vector.
 * \retval true if all of the components of the vector are zero, false otherwise.
 * \retval false one or more of the components of the vector are non-zero.
 * \note This function uses \ref AL_EPSILON as the epsilon value for floating point comparisons.
 */
bool    ac_vec4_is_zero(const ac_vec4* v);
/**
 * \ingroup vec4
 * \brief Checks if a vector has NaN components.
 * \param[in] v The vector.
 * \return True if any of the components of the vector are NaN, false otherwise.
 */
bool    ac_vec4_is_nan(const ac_vec4* v);
/**
 * \ingroup vec4
 * \brief Checks if two vectors are equal.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \retval true if all of the components of the vectors are equal, false otherwise.
 * \retval false if one or more of the components of the vectors are not equal.
 * \retval false if either vector has NaN components.
 * \note This function uses \ref AC_EPSILON as the epsilon value for floating point comparisons.
 */
bool    ac_vec4_is_equal(const ac_vec4* a, const ac_vec4* b);
/**
 * \ingroup vec4
 * \brief Adds two vectors: a + b.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The sum of the two vectors.
 */
ac_vec4 ac_vec4_add(const ac_vec4* a, const ac_vec4* b);
/**
 * \ingroup vec4
 * \brief Subtracts two vectors: a - b.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The difference of the two vectors.
 */
ac_vec4 ac_vec4_sub(const ac_vec4* a, const ac_vec4* b);
/**
 * \ingroup vec4
 * \brief Negates a vector.
 * \param[in] v The vector to negate.
 * \return The negated vector.
 */
ac_vec4 ac_vec4_negate(const ac_vec4* v);
/**
 * \ingroup vec4
 * \brief Multiplies a vector by a scalar: a * scalar.
 * \param[in] v The vector.
 * \param[in] scalar The scalar.
 * \return The scaled vector.
 */
ac_vec4 ac_vec4_scale(const ac_vec4* v, float scalar);
/**
 * \ingroup vec4
 * \brief Computes the dot product of two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The dot product of the two vectors.
 */
float   ac_vec4_dot(const ac_vec4* a, const ac_vec4* b);
/**
 * \ingroup vec4
 * \brief Computes the length of a vector.
 * \param[in] v The vector.
 * \return The length of the vector.
 */
float   ac_vec4_magnitude(const ac_vec4* v);
/**
 * \ingroup vec4
 * \brief Normalizes a vector.
 * \param[in] v The vector.
 * \return The normalized vector.
 * \details
 * If the vector is of zero length or NaN, the returned vector will have NaN components.
 */
ac_vec4 ac_vec4_normalize(const ac_vec4* v);

//--------------------------------------------------------------------------------------------------
// int
//--------------------------------------------------------------------------------------------------

/**
 * \ingroup ivec4
 * \def INT_INVALID
 * \brief A sentinel value for an invalid integer.
 */
#define INT_INVALID INT_MIN

/**
 * \ingroup ivec4
 * \union ac_ivec4
 * \brief A 4-component vector of type int.
 * \details
 * A standard 4-component vector of type int which provides 'aliases' for
 * axes, colors, and elements.
 */
typedef union ac_ivec4
{
    struct
    {
        int x, y, z, w;
    };
    struct
    {
        int r, g, b, a;
    };
    int data[4];
} ac_ivec4;

/**
 * \ingroup ivec4
 * \brief Creates a vector with all components set to zero.
 * \return A vector with all components set to zero.
 */
ac_ivec4 ac_ivec4_zero(void);
/**
 * \ingroup ivec4
 * \brief Creates a vector with all components set to an invalid sentinel.
 * \return A vector with all components set to an invalid sentinel.
 * \see INT_INVALID
 */
ac_ivec4 ac_ivec4_invalid(void);
/**
 * \ingroup ivec4
 * \brief Checks if a vector has zero components.
 * \param[in] v The vector.
 * \retval true if all of the components of the vector are zero.
 * \retval false one or more of the components of the vector are non-zero.
 */
bool     ac_ivec4_is_zero(const ac_ivec4* v);
/**
 * \ingroup ivec4
 * \brief Checks if a vector has invalid components.
 * \param[in] v The vector.
 * \retval true if any of the components of the vector are invalid.
 * \retval false if all of the components of the vector are valid.
 * \see INT_INVALID
 */
bool     ac_ivec4_is_invalid(const ac_ivec4* v);
/**
 * \ingroup ivec4
 * \brief Checks if two vectors are equal.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \retval true if all of the components of the vectors are equal.
 * \retval false if one or more of the components of the vectors are not equal.
 */
bool     ac_ivec4_is_equal(const ac_ivec4* a, const ac_ivec4* b);
/**
 * \ingroup ivec4
 * \brief Adds two vectors: a + b.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The sum of the two vectors.
 * \note If either vector has invalid components, the result will be the invalid ivec4.
 * \see ac_ivec4_is_invalid
 */
ac_ivec4 ac_ivec4_add(const ac_ivec4* a, const ac_ivec4* b);
/**
 * \ingroup ivec4
 * \brief Subtracts two vectors: a - b.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The difference of the two vectors.
 * \note If either vector has invalid components, the result will be the invalid ivec4.
 * \see ac_ivec4_is_invalid
 */
ac_ivec4 ac_ivec4_sub(const ac_ivec4* a, const ac_ivec4* b);
/**
 * \ingroup ivec4
 * \brief Negates a vector.
 * \param[in] v The vector to negate.
 * \return The negated vector.
 * \note If the vector has invalid components, the result will be the invalid ivec4.
 * \see ac_ivec4_is_invalid
 */
ac_ivec4 ac_ivec4_negate(const ac_ivec4* v);
/**
 * \ingroup ivec4
 * \brief Multiplies a vector by a scalar: a * scalar.
 * \param[in] v The vector.
 * \param[in] scalar The scalar.
 * \return The scaled vector.
 * \note If the vector has invalid components, the result will be the invalid ivec4.
 * \see ac_ivec4_is_invalid
 */
ac_ivec4 ac_ivec4_scale(const ac_ivec4* v, int scalar);
/**
 * \ingroup ivec4
 * \brief Divides a vector by a scalar: a / scalar.
 * \param[in] v The vector.
 * \param[in] scalar The scalar.
 * \return The scaled vector.
 * \note If the vector has invalid components, the result will be the invalid ivec4.
 * \note If the scalar is zero, the result will be the invalid ivec4.
 * \see ac_ivec4_is_invalid
 * \details
 * This function uses integer division, so the result will be truncated.
 * The result of this integer division is defined by the C standard to be the
 * floor of the division. If you need a different behavior for this rounding,
 * use \ref ac_ivec4_divide_ext.
 */
ac_ivec4 ac_ivec4_divide(const ac_ivec4* v, int scalar);
/**
 * \ingroup ivec4
 * \brief Divides a vector by a scalar: a / scalar.
 * \param[in] v The vector.
 * \param[in] scalar The scalar.
 * \param[in] rounding_func Used to rount the result of the division.
 * \return The scaled vector.
 * \note If the vector has invalid components, the result will be the invalid ivec4.
 * \note If the scalar is zero, the result will be the invalid ivec4.
 * \see ac_ivec4_is_invalid
 * \details
 * This function uses a rounding function to determine the result of the division.
 * The rounding function should take a float as input and return an integer.
 * The result of the division is then rounded using this function.
 * If the result of the standard C integer division is desired, use \ref ac_ivec4_divide.
 */
ac_ivec4 ac_ivec4_divide_ext(const ac_ivec4* v, int scalar, int (*rounding_func)(float));
/**
 * \ingroup ivec4
 * \brief Computes the dot product of two vectors.
 * \param[in] a The first vector.
 * \param[in] b The second vector.
 * \return The dot product of the two vectors.
 * \note If either vector has invalid components, the result will be \ref INT_INVALID.
 */
int      ac_ivec4_dot(const ac_ivec4* a, const ac_ivec4* b);

#ifdef __cplusplus
}
#endif
