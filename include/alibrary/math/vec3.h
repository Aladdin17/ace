#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * \typedef Vec3
	 * \brief A 3-component vector of type float.
	 * \details
	 * The underlying structure is a 3-element array but also provides access by axis and color.
	*/
typedef union Vec3
{
	struct { float x, y, z; };   ///< Access by axis.
	struct { float r, g, b; };   ///< Access by color.
	float comp[3];               ///< Access by element.
} Vec3;


//--------------------------------------------------------------------------------------------------
// Vec3
//--------------------------------------------------------------------------------------------------

	/**
	 * \brief Adds two vectors: a + b.
	 * \param[in] a The first vector.
	 * \param[in] b The second vector.
	 * \return The sum of the two vectors.
	*/
Vec3 Vec3Add(const Vec3* a, const Vec3* b);
	/**
	 * \brief Subtracts two vectors: a - b.
	 * \param[in] a The first vector.
	 * \param[in] b The second vector.
	 * \return The difference of the two vectors.
	*/
Vec3 Vec3Sub(const Vec3* a, const Vec3* b);
	/**
	 * \brief Negates a vector.
	 * \param[in] a The vector to negate.
	 * \return The negated vector.
	*/
Vec3 Vec3Neg(const Vec3* v);
	/**
	 * \brief Multiplies a vector by a scalar: a * scalar.
	 * \param[in] a The vector.
	 * \param[in] scalar The scalar.
	 * \return The scaled vector.
	*/
Vec3 Vec3Scale(const Vec3* v, float scalar);
	/**
	 * \brief Computes the dot product of two vectors.
	 * \param[in] a The first vector.
	 * \param[in] b The second vector.
	 * \return The dot product of the two vectors.
	*/
float Vec3Dot(const Vec3* a, const Vec3* b);
	/**
	 * \brief Computes the cross product of two vectors: a x b.
	 * \param[in] a The first vector.
	 * \param[in] b The second vector.
	 * \return The cross product of the two vectors.
	*/
Vec3 Vec3Cross(const Vec3* a, const Vec3* b);
	/**
	 * \brief Computes the length of a vector.
	 * \param[in] a The vector.
	 * \return The length of the vector.
	*/
float Vec3Lenth(const Vec3* v);
	/**
	 * \brief Normalizes a vector.
	 * \param[in] a The vector.
	 * \return The normalized vector.
	 * \note If the length of the vector is 0, the components of the returned vector are NaN.
	 * \see Vec3IsNaN
	*/
Vec3 Vec3Normalize(const Vec3* v);
	/**
	 * \brief Checks if a vector has NaN components.
	 * \param[in] a The vector.
	 * \return True if any of the components of the vector are NaN, false otherwise.
	*/
bool Vec3IsNaN(const Vec3* v);

#ifdef __cplusplus
}
#endif
