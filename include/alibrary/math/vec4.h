#pragma once
#include <stdbool.h>
#include <stdint.h>

	/**
	 * \typedef Vec4f
	 * \brief A 4-component vector.
	 * \details
	 * The underlying structure is a 4-element array but also provides access by axis and color.
	*/
typedef union
{
	float comp[4];                  ///< Access by element.
	struct { float x, y, z, w; };   ///< Access by axis.
	struct { float r, g, b, a; };   ///< Access by color.
} Vec4f;


	/**
	 * \brief Adds two vectors: a + b.
	 * \param[in] a The first vector.
	 * \param[in] b The second vector.
	 * \return The sum of the two vectors.
	*/
Vec4f Vec4fAdd(const Vec4f* a, const Vec4f* b);
	/**
	 * \brief Subtracts two vectors: a - b.
	 * \param[in] a The first vector.
	 * \param[in] b The second vector.
	 * \return The difference of the two vectors.
	*/
Vec4f Vec4fSub(const Vec4f* a, const Vec4f* b);
	/**
	 * \brief Negates a vector.
	 * \param[in] a The vector to negate.
	 * \return The negated vector.
	*/
Vec4f Vec4fNeg(const Vec4f* v);
	/**
	 * \brief Multiplies a vector by a scalar: a * scalar.
	 * \param[in] a The vector.
	 * \param[in] scalar The scalar.
	 * \return The scaled vector.
	*/
Vec4f Vec4fMul(const Vec4f* v, float scalar);
	/**
	 * \brief Computes the dot product of two vectors.
	 * \param[in] a The first vector.
	 * \param[in] b The second vector.
	 * \return The dot product of the two vectors.
	*/
float Vec4fDot(const Vec4f* a, const Vec4f* b);
	/**
	 * \brief Computes the cross product of two vectors: a x b.
	 * \param[in] a The first vector.
	 * \param[in] b The second vector.
	 * \return The cross product of the two vectors.
	*/
Vec4f Vec4fCross(const Vec4f* a, const Vec4f* b);
	/**
	 * \brief Computes the length of a vector.
	 * \param[in] a The vector.
	 * \return The length of the vector.
	*/
float Vec4fLenth(const Vec4f* v);
	/**
	 * \brief Normalizes a vector.
	 * \param[in] a The vector.
	 * \return The normalized vector.
	 * \note If the length of the vector is 0, the components of the returned vector are NaN.
	 * \see Vec4fIsNaN
	*/
Vec4f Vec4fNormalize(const Vec4f* v);
	/**
	 * \brief Checks if a vector has NaN components.
	 * \param[in] a The vector.
	 * \return True if any of the components of the vector are NaN, false otherwise.
	*/
bool Vec4fIsNaN(const Vec4f* v);
