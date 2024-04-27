/**
 * \file
 * \author Christien Alden
 * \brief 3-component vector types and functions implementation.
 */
#include <alibrary/math/math.h>
#include <alibrary/math/vec3.h>
#include <math.h>

vec3 vec3_zero(void)
{
    return (vec3){ 0.0f, 0.0f, 0.0f };
}

vec3 vec3_nan(void)
{
    return (vec3){ NAN, NAN, NAN };
}

bool vec3_is_zero(const vec3* v)
{
    return vec3_magnitude(v) <= AL_EPSILON;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
bool vec3_is_nan(const vec3* v)
{
    return (isnan(v->x) || isnan(v->y) || isnan(v->z));
}

bool vec3_is_equal(const vec3* a, const vec3* b)
{
    vec3 diff = vec3_sub(a, b);
    return vec3_is_zero(&diff);
}

vec3 vec3_add(const vec3* a, const vec3* b)
{
    return (vec3){
        {a->x + b->x, a->y + b->y, a->z + b->z}
    };
}

vec3 vec3_sub(const vec3* a, const vec3* b)
{
    return (vec3){
        {a->x - b->x, a->y - b->y, a->z - b->z}
    };
}

vec3 vec3_negate(const vec3* v)
{
    return (vec3){
        {-v->x, -v->y, -v->z}
    };
}

vec3 vec3_scale(const vec3* v, float scalar)
{
    return (vec3){
        {v->x * scalar, v->y * scalar, v->z * scalar}
    };
}

float vec3_dot(const vec3* a, const vec3* b)
{
    return (a->x * b->x + a->y * b->y + a->z * b->z);
}

vec3 vec3_cross(const vec3* a, const vec3* b)
{
    return (vec3){
        {a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z, a->x * b->y - a->y * b->x}
    };
}

float vec3_magnitude(const vec3* v)
{
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

vec3 vec3_normalize(const vec3* v)
{
    // normalization of a zero or NaN vector is undefined
    if ( vec3_is_nan(v) || vec3_is_zero(v) )
    {
        return vec3_nan();
    }

    // calculate the inverse magnitude and scale the vector
    float invMagnitude = 1.0f / vec3_magnitude(v);
    return vec3_scale(v, invMagnitude);
}
