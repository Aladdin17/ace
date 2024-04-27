/**
 * \file
 * \author Christien Alden
 * \brief 3-component vector types and functions implementation.
 */
#include <alibrary/math/math.h>
#include <alibrary/math/vec3.h>
#include <math.h>

//--------------------------------------------------------------------------------------------------
// float
//--------------------------------------------------------------------------------------------------

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
    return (vec3){ a->x + b->x, a->y + b->y, a->z + b->z };
}

vec3 vec3_sub(const vec3* a, const vec3* b)
{
    return (vec3){ a->x - b->x, a->y - b->y, a->z - b->z };
}

vec3 vec3_negate(const vec3* v)
{
    return (vec3){ -v->x, -v->y, -v->z };
}

vec3 vec3_scale(const vec3* v, float scalar)
{
    return (vec3){ v->x * scalar, v->y * scalar, v->z * scalar };
}

float vec3_dot(const vec3* a, const vec3* b)
{
    return (a->x * b->x + a->y * b->y + a->z * b->z);
}

vec3 vec3_cross(const vec3* a, const vec3* b)
{
    return (vec3
    ){ a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z, a->x * b->y - a->y * b->x };
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

//--------------------------------------------------------------------------------------------------
// int
//--------------------------------------------------------------------------------------------------

ivec3 ivec3_zero(void)
{
    return (ivec3){ 0, 0, 0 };
}

ivec3 ivec3_nan(void)
{
    return (ivec3){ INT_NAN, INT_NAN, INT_NAN };
}

bool ivec3_is_zero(const ivec3* v)
{
    return (v->x == 0 && v->y == 0 && v->z == 0);
}

bool ivec3_is_nan(const ivec3* v)
{
    return (v->x == INT_NAN || v->y == INT_NAN || v->z == INT_NAN);
}

bool ivec3_is_equal(const ivec3* a, const ivec3* b)
{
    if ( ivec3_is_nan(a) || ivec3_is_nan(b) )
    {
        return false;
    }

    return (a->x == b->x && a->y == b->y && a->z == b->z);
}

ivec3 ivec3_add(const ivec3* a, const ivec3* b)
{
    if ( ivec3_is_nan(a) || ivec3_is_nan(b) )
    {
        return ivec3_nan();
    }

    return (ivec3){ a->x + b->x, a->y + b->y, a->z + b->z };
}

ivec3 ivec3_sub(const ivec3* a, const ivec3* b)
{
    if ( ivec3_is_nan(a) || ivec3_is_nan(b) )
    {
        return ivec3_nan();
    }

    return (ivec3){ a->x - b->x, a->y - b->y, a->z - b->z };
}

ivec3 ivec3_negate(const ivec3* v)
{
    if ( ivec3_is_nan(v) )
    {
        return ivec3_nan();
    }

    return (ivec3){ -v->x, -v->y, -v->z };
}

ivec3 ivec3_scale(const ivec3* v, int scalar)
{
    if ( ivec3_is_nan(v) )
    {
        return ivec3_nan();
    }

    return (ivec3){ v->x * scalar, v->y * scalar, v->z * scalar };
}

ivec3 ivec3_divide(const ivec3* v, int scalar)
{
    if ( ivec3_is_nan(v) || scalar == 0 )
    {
        return ivec3_nan();
    }

    return (ivec3){ v->x / scalar, v->y / scalar, v->z / scalar };
}

ivec3 ivec3_divide_ext(const ivec3* v, int scalar, rounding_func func)
{
    if ( ivec3_is_nan(v) || scalar == 0 )
    {
        return ivec3_nan();
    }

    // convert to float
    float scalarf = (float) scalar;
    float xf      = (float) v->x;
    float yf      = (float) v->y;
    float zf      = (float) v->z;

    // divide and round
    return (ivec3){ func(xf / scalarf), func(yf / scalarf), func(zf / scalarf) };
}

int ivec3_dot(const ivec3* a, const ivec3* b)
{
    if ( ivec3_is_nan(a) || ivec3_is_nan(b) )
    {
        return INT_NAN;
    }

    return (a->x * b->x + a->y * b->y + a->z * b->z);
}

ivec3 ivec3_cross(const ivec3* a, const ivec3* b)
{
    if ( ivec3_is_nan(a) || ivec3_is_nan(b) )
    {
        return ivec3_nan();
    }

    return (ivec3
    ){ a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z, a->x * b->y - a->y * b->x };
}
