/**
 * \file
 * \author Christien Alden
 * \brief 3-component vector types and functions implementation.
 */
#include <ace/math/math.h>
#include <ace/math/vec3.h>
#include <math.h>

//--------------------------------------------------------------------------------------------------
// float
//--------------------------------------------------------------------------------------------------

ac_vec3 ac_vec3_zero(void)
{
    return (ac_vec3){ 0.0f, 0.0f, 0.0f };
}

ac_vec3 ac_vec3_nan(void)
{
    return (ac_vec3){ NAN, NAN, NAN };
}

bool ac_vec3_is_zero(const ac_vec3* v)
{
    return ac_vec3_magnitude(v) <= AC_EPSILON;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
bool ac_vec3_is_nan(const ac_vec3* v)
{
    return (isnan(v->x) || isnan(v->y) || isnan(v->z));
}

bool ac_vec3_is_equal(const ac_vec3* a, const ac_vec3* b)
{
    ac_vec3 diff = ac_vec3_sub(a, b);
    return ac_vec3_is_zero(&diff);
}

ac_vec3 ac_vec3_add(const ac_vec3* a, const ac_vec3* b)
{
    return (ac_vec3){ a->x + b->x, a->y + b->y, a->z + b->z };
}

ac_vec3 ac_vec3_sub(const ac_vec3* a, const ac_vec3* b)
{
    return (ac_vec3){ a->x - b->x, a->y - b->y, a->z - b->z };
}

ac_vec3 ac_vec3_negate(const ac_vec3* v)
{
    return (ac_vec3){ -v->x, -v->y, -v->z };
}

ac_vec3 ac_vec3_scale(const ac_vec3* v, float scalar)
{
    return (ac_vec3){ v->x * scalar, v->y * scalar, v->z * scalar };
}

float ac_vec3_dot(const ac_vec3* a, const ac_vec3* b)
{
    return (a->x * b->x + a->y * b->y + a->z * b->z);
}

ac_vec3 ac_vec3_cross(const ac_vec3* a, const ac_vec3* b)
{
    return (ac_vec3
    ){ a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z, a->x * b->y - a->y * b->x };
}

float ac_vec3_magnitude(const ac_vec3* v)
{
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

ac_vec3 ac_vec3_normalize(const ac_vec3* v)
{
    // normalization of a zero or NaN vector is undefined
    if ( ac_vec3_is_nan(v) || ac_vec3_is_zero(v) )
    {
        return ac_vec3_nan();
    }

    // calculate the inverse magnitude and scale the vector
    float invMagnitude = 1.0f / ac_vec3_magnitude(v);
    return ac_vec3_scale(v, invMagnitude);
}

//--------------------------------------------------------------------------------------------------
// int
//--------------------------------------------------------------------------------------------------

ac_ivec3 ac_ivec3_zero(void)
{
    return (ac_ivec3){ 0, 0, 0 };
}

ac_ivec3 ac_ivec3_invalid(void)
{
    return (ac_ivec3){ INT_INVALID, INT_INVALID, INT_INVALID };
}

bool ac_ivec3_is_zero(const ac_ivec3* v)
{
    return (v->x == 0 && v->y == 0 && v->z == 0);
}

bool ac_ivec3_is_invalid(const ac_ivec3* v)
{
    return (v->x == INT_INVALID || v->y == INT_INVALID || v->z == INT_INVALID);
}

bool ac_ivec3_is_equal(const ac_ivec3* a, const ac_ivec3* b)
{
    if ( ac_ivec3_is_invalid(a) || ac_ivec3_is_invalid(b) )
    {
        return false;
    }

    return (a->x == b->x && a->y == b->y && a->z == b->z);
}

ac_ivec3 ac_ivec3_add(const ac_ivec3* a, const ac_ivec3* b)
{
    if ( ac_ivec3_is_invalid(a) || ac_ivec3_is_invalid(b) )
    {
        return ac_ivec3_invalid();
    }

    return (ac_ivec3){ a->x + b->x, a->y + b->y, a->z + b->z };
}

ac_ivec3 ac_ivec3_sub(const ac_ivec3* a, const ac_ivec3* b)
{
    if ( ac_ivec3_is_invalid(a) || ac_ivec3_is_invalid(b) )
    {
        return ac_ivec3_invalid();
    }

    return (ac_ivec3){ a->x - b->x, a->y - b->y, a->z - b->z };
}

ac_ivec3 ac_ivec3_negate(const ac_ivec3* v)
{
    if ( ac_ivec3_is_invalid(v) )
    {
        return ac_ivec3_invalid();
    }

    return (ac_ivec3){ -v->x, -v->y, -v->z };
}

ac_ivec3 ac_ivec3_scale(const ac_ivec3* v, int scalar)
{
    if ( ac_ivec3_is_invalid(v) )
    {
        return ac_ivec3_invalid();
    }

    return (ac_ivec3){ v->x * scalar, v->y * scalar, v->z * scalar };
}

ac_ivec3 ac_ivec3_divide(const ac_ivec3* v, int scalar)
{
    if ( ac_ivec3_is_invalid(v) || scalar == 0 )
    {
        return ac_ivec3_invalid();
    }

    return (ac_ivec3){ v->x / scalar, v->y / scalar, v->z / scalar };
}

ac_ivec3 ac_ivec3_divide_ext(const ac_ivec3* v, int scalar, int (*rounding_func)(float))
{
    if ( ac_ivec3_is_invalid(v) || scalar == 0 )
    {
        return ac_ivec3_invalid();
    }

    // convert to float
    float scalarf = (float) scalar;
    float xf      = (float) v->x;
    float yf      = (float) v->y;
    float zf      = (float) v->z;

    // divide and round
    return (ac_ivec3
    ){ rounding_func(xf / scalarf), rounding_func(yf / scalarf), rounding_func(zf / scalarf) };
}

int ac_ivec3_dot(const ac_ivec3* a, const ac_ivec3* b)
{
    if ( ac_ivec3_is_invalid(a) || ac_ivec3_is_invalid(b) )
    {
        return INT_INVALID;
    }

    return (a->x * b->x + a->y * b->y + a->z * b->z);
}

ac_ivec3 ac_ivec3_cross(const ac_ivec3* a, const ac_ivec3* b)
{
    if ( ac_ivec3_is_invalid(a) || ac_ivec3_is_invalid(b) )
    {
        return ac_ivec3_invalid();
    }

    return (ac_ivec3
    ){ a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z, a->x * b->y - a->y * b->x };
}
