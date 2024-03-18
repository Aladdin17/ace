/**
 * \file
 * \author Christien Alden
 * \brief 3-component vector types and functions implementation.
 */
#include <alibrary/math/vec3.h>
#include <math.h>

/**
 * \def AML_EPSILON
 * \brief An epsilon value for floating point comparisons against zero.
 */
#define AML_EPSILON 1e-5f

//--------------------------------------------------------------------------------------------------
// Float
//--------------------------------------------------------------------------------------------------

Vec3 Vec3Add(const Vec3* a, const Vec3* b)
{
    return (Vec3){
        {a->x + b->x, a->y + b->y, a->z + b->z}
    };
}

Vec3 Vec3Sub(const Vec3* a, const Vec3* b)
{
    return (Vec3){
        {a->x - b->x, a->y - b->y, a->z - b->z}
    };
}

Vec3 Vec3Neg(const Vec3* v)
{
    return (Vec3){
        {-v->x, -v->y, -v->z}
    };
}

Vec3 Vec3Scale(const Vec3* v, float scalar)
{
    return (Vec3){
        {v->x * scalar, v->y * scalar, v->z * scalar}
    };
}

float Vec3Dot(const Vec3* a, const Vec3* b)
{
    return (a->x * b->x + a->y * b->y + a->z * b->z);
}

Vec3 Vec3Cross(const Vec3* a, const Vec3* b)
{
    return (Vec3){
        {a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z, a->x * b->y - a->y * b->x}
    };
}

float Vec3Lenth(const Vec3* v)
{
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

Vec3 Vec3Normalize(const Vec3* v)
{
    // naive check for zero vector
    float magnitude = Vec3Lenth(v);
    if ( fabsf(magnitude) < AML_EPSILON )
    {
        return (Vec3){ NAN, NAN, NAN };
    }

    // normalize
    float invMagnitude = 1.0f / magnitude;
    return Vec3Scale(v, invMagnitude);
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
bool Vec3IsNaN(const Vec3* v)
{
    return (isnan(v->x) || isnan(v->y) || isnan(v->z));
}
