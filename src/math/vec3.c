/**
 * \file
 * \author Christien Alden
 * \brief 3-component vector types and functions implementation.
 */
#include <alibrary/math/vec3.h>
#include <math.h>

Vec3 vec3_zero(void)
{
    return (Vec3){ 0.0f, 0.0f, 0.0f };
}

Vec3 vec3_nan(void)
{
    return (Vec3){ NAN, NAN, NAN };
}

bool vec3_is_zero(const Vec3* v)
{
    return vec3_magnitude(v) <= AL_EPSILON;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
bool vec3_is_nan(const Vec3* v)
{
    return (isnan(v->x) || isnan(v->y) || isnan(v->z));
}

Vec3 vec3_add(const Vec3* a, const Vec3* b)
{
    return (Vec3){
        {a->x + b->x, a->y + b->y, a->z + b->z}
    };
}

Vec3 vec3_sub(const Vec3* a, const Vec3* b)
{
    return (Vec3){
        {a->x - b->x, a->y - b->y, a->z - b->z}
    };
}

Vec3 vec3_negate(const Vec3* v)
{
    return (Vec3){
        {-v->x, -v->y, -v->z}
    };
}

Vec3 vec3_scale(const Vec3* v, float scalar)
{
    return (Vec3){
        {v->x * scalar, v->y * scalar, v->z * scalar}
    };
}

float vec3_dot(const Vec3* a, const Vec3* b)
{
    return (a->x * b->x + a->y * b->y + a->z * b->z);
}

Vec3 vec3_cross(const Vec3* a, const Vec3* b)
{
    return (Vec3){
        {a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z, a->x * b->y - a->y * b->x}
    };
}

float vec3_magnitude(const Vec3* v)
{
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

Vec3 vec3_normalize(const Vec3* v)
{
    // guard against zero or NaN vectors
    if ( vec3_is_nan(v) || vec3_is_zero(v) )
    {
        return vec3_nan();
    }

    // calculate the inverse magnitude and scale the vector
    float invMagnitude = 1.0f / vec3_magnitude(v);
    return vec3_scale(v, invMagnitude);
}

//--------------------------------------------------------------------------------------------------
// Utility functions
//--------------------------------------------------------------------------------------------------

float vec3_angle(const Vec3* a, const Vec3* b)
{
    // guard against NaN vectors
    if ( vec3_is_nan(a) || vec3_is_nan(b) )
    {
        return NAN;
    }

    // guard against zero vectors
    if ( vec3_is_zero(a) || vec3_is_zero(b) )
    {
        return NAN;
    }

    // the dot product can be defined as: dot = |a| * |b| * cos(theta)
    // therefore, theta = acos(dot / (|a| * |b|))
    float dot  = vec3_dot(a, b);
    float magA = vec3_magnitude(a);
    float magB = vec3_magnitude(b);
    return acosf(dot / (magA * magB));
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
Vec3 vec3_reflect(const Vec3* incoming, const Vec3* normal)
{
    // to reflect a vector v about a normal n, we want to project v onto n
    // and then subtract the projection from v twice, which is the same as
    // subtracting 2 times the projection from v. This is equivalent to:
    // reflect = v - 2 * (v . n) * n

    // check incoming vector for validity and return a NaN vector if it is invalid
    if ( vec3_is_nan(incoming) || vec3_is_zero(incoming) )
    {
        return vec3_nan();
    }
    // normalize the normal to ensure the projection is correct and
    // return a NaN vector if the normal is zero
    Vec3 n_normalized = vec3_normalize(normal);
    if ( vec3_is_nan(&n_normalized) )
    {
        return vec3_nan();
    }

    // the projection modifier is used to scale the projection of v onto n
    static const float projection_modifier = 2.0f;

    // calculate the projection of v onto n
    float projection = vec3_dot(incoming, &n_normalized);
    Vec3  scaled_n   = vec3_scale(&n_normalized, projection_modifier * projection);
    return vec3_sub(incoming, &scaled_n);
}
