#pragma once
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 * \typedef Vec2f
	 * \brief A 2-component vector.
	 * \details
	 * The underlying structure is a 2-element array but also provides access by axis and texcoord.
	*/
typedef union
{
	float comp[2];            ///< Access by element.
	struct { float x, y; };   ///< Access by axis.
	struct { float u, v; };   ///< Access by texture coordinates.
} Vec2f;


	/**
	 * \brief Adds two vectors: a + b.
	 * \param[in] a The first vector.
	 * \param[in] b The second vector.
	 * \return The sum of the two vectors.
	*/
Vec2f Vec2fAdd(const Vec2f* a, const Vec2f* b);
	/**
	 * \brief Subtracts two vectors: a - b.
	 * \param[in] a The first vector.
	 * \param[in] b The second vector.
	 * \return The difference of the two vectors.
	*/
Vec2f Vec2fSub(const Vec2f* a, const Vec2f* b);
	/**
	 * \brief Negates a vector.
	 * \param[in] a The vector to negate.
	 * \return The negated vector.
	*/
Vec2f Vec2fNeg(const Vec2f* v);
	/**
	 * \brief Multiplies a vector by a scalar: a * scalar.
	 * \param[in] a The vector.
	 * \param[in] scalar The scalar.
	 * \return The scaled vector.
	*/
Vec2f Vec2fMul(const Vec2f* v, float scalar);
	/**
	 * \brief Computes the dot product of two vectors.
	 * \param[in] a The first vector.
	 * \param[in] b The second vector.
	 * \return The dot product of the two vectors.
	*/
float Vec2fDot(const Vec2f* a, const Vec2f* b);
	/**
	 * \brief Computes the cross product of two vectors: a x b.
	 * \param[in] a The first vector.
	 * \param[in] b The second vector.
	 * \return The cross product of the two vectors.
	*/
Vec2f Vec2fCross(const Vec2f* a, const Vec2f* b);
	/**
	 * \brief Computes the length of a vector.
	 * \param[in] a The vector.
	 * \return The length of the vector.
	*/
float Vec2fLenth(const Vec2f* v);
	/**
	 * \brief Normalizes a vector.
	 * \param[in] a The vector.
	 * \return The normalized vector.
	 * \note If the length of the vector is 0, the components of the returned vector are NaN.
	 * \see Vec2fIsNaN
	*/
Vec2f Vec2fNormalize(const Vec2f* v);
	/**
	 * \brief Checks if a vector has NaN components.
	 * \param[in] a The vector.
	 * \return True if any of the components of the vector are NaN, false otherwise.
	*/
bool Vec2fIsNaN(const Vec2f* v);

#ifdef __cplusplus
}
#endif
