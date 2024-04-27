/**
 * \file
 * \author Christien Alden
 * \brief 4-component vector types and functions implementation.
 */
#include <ace/math/math.h>
#include <ace/math/vec4.h>
#include <math.h>

//--------------------------------------------------------------------------------------------------
// float
//--------------------------------------------------------------------------------------------------

ac_vec4 ac_vec4_zero(void)
{
    return (ac_vec4){ 0.0f, 0.0f, 0.0f, 0.0f };
}

ac_vec4 ac_vec4_nan(void)
{
    return (ac_vec4){ NAN, NAN, NAN, NAN };
}

bool ac_vec4_is_zero(const ac_vec4* v)
{
    return ac_vec4_magnitude(v) <= AC_EPSILON;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
bool ac_vec4_is_nan(const ac_vec4* v)
{
    return (isnan(v->x) || isnan(v->y) || isnan(v->z) || isnan(v->w));
}

bool ac_vec4_is_equal(const ac_vec4* a, const ac_vec4* b)
{
    ac_vec4 diff = ac_vec4_sub(a, b);
    return ac_vec4_is_zero(&diff);
}

ac_vec4 ac_vec4_add(const ac_vec4* a, const ac_vec4* b)
{
    return (ac_vec4){ a->x + b->x, a->y + b->y, a->z + b->z, a->w + b->w };
}

ac_vec4 ac_vec4_sub(const ac_vec4* a, const ac_vec4* b)
{
    return (ac_vec4){ a->x - b->x, a->y - b->y, a->z - b->z, a->w - b->w };
}

ac_vec4 ac_vec4_negate(const ac_vec4* v)
{
    return (ac_vec4){ -v->x, -v->y, -v->z, -v->w };
}

ac_vec4 ac_vec4_scale(const ac_vec4* v, float scalar)
{
    return (ac_vec4){ v->x * scalar, v->y * scalar, v->z * scalar, v->w * scalar };
}

float ac_vec4_dot(const ac_vec4* a, const ac_vec4* b)
{
    return (a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w);
}

float ac_vec4_magnitude(const ac_vec4* v)
{
    return sqrtf(ac_vec4_dot(v, v));
}

ac_vec4 ac_vec4_normalize(const ac_vec4* v)
{
    float magnitude = ac_vec4_magnitude(v);
    if ( magnitude <= AC_EPSILON )
    {
        return ac_vec4_nan();
    }

    // calculate the inverse magnitude and scale the vector
    float inv_magnitude = 1.0f / magnitude;
    return ac_vec4_scale(v, inv_magnitude);
}

//--------------------------------------------------------------------------------------------------
// int
//--------------------------------------------------------------------------------------------------

ac_ivec4 ac_ivec4_zero(void)
{
    return (ac_ivec4){ 0, 0, 0, 0 };
}

ac_ivec4 ac_ivec4_invalid(void)
{
    return (ac_ivec4){ INT_INVALID, INT_INVALID, INT_INVALID, INT_INVALID };
}

bool ac_ivec4_is_zero(const ac_ivec4* v)
{
    return (v->x == 0 && v->y == 0 && v->z == 0 && v->w == 0);
}

bool ac_ivec4_is_invalid(const ac_ivec4* v)
{
    return (
        v->x == INT_INVALID || v->y == INT_INVALID || v->z == INT_INVALID || v->w == INT_INVALID
    );
}

bool ac_ivec4_is_equal(const ac_ivec4* a, const ac_ivec4* b)
{
    if ( ac_ivec4_is_invalid(a) || ac_ivec4_is_invalid(b) )
    {
        return false;
    }

    return (a->x == b->x && a->y == b->y && a->z == b->z && a->w == b->w);
}

ac_ivec4 ac_ivec4_add(const ac_ivec4* a, const ac_ivec4* b)
{
    if ( ac_ivec4_is_invalid(a) || ac_ivec4_is_invalid(b) )
    {
        return ac_ivec4_invalid();
    }

    return (ac_ivec4){ a->x + b->x, a->y + b->y, a->z + b->z, a->w + b->w };
}

ac_ivec4 ac_ivec4_sub(const ac_ivec4* a, const ac_ivec4* b)
{
    if ( ac_ivec4_is_invalid(a) || ac_ivec4_is_invalid(b) )
    {
        return ac_ivec4_invalid();
    }

    return (ac_ivec4){ a->x - b->x, a->y - b->y, a->z - b->z, a->w - b->w };
}

ac_ivec4 ac_ivec4_negate(const ac_ivec4* v)
{
    if ( ac_ivec4_is_invalid(v) )
    {
        return ac_ivec4_invalid();
    }

    return (ac_ivec4){ -v->x, -v->y, -v->z, -v->w };
}

ac_ivec4 ac_ivec4_scale(const ac_ivec4* v, int scalar)
{
    if ( ac_ivec4_is_invalid(v) )
    {
        return ac_ivec4_invalid();
    }
    return (ac_ivec4){ v->x * scalar, v->y * scalar, v->z * scalar, v->w * scalar };
}

ac_ivec4 ac_ivec4_divide(const ac_ivec4* v, int scalar)
{
    if ( ac_ivec4_is_invalid(v) || scalar == 0 )
    {
        return ac_ivec4_invalid();
    }

    return (ac_ivec4){ v->x / scalar, v->y / scalar, v->z / scalar, v->w / scalar };
}

ac_ivec4 ac_ivec4_divide_ext(const ac_ivec4* v, int scalar, int (*rounding_func)(float))
{
    if ( ac_ivec4_is_invalid(v) || scalar == 0 )
    {
        return ac_ivec4_invalid();
    }

    float scalarf = (float) scalar;
    float xf      = (float) v->x;
    float yf      = (float) v->y;
    float zf      = (float) v->z;
    float wf      = (float) v->w;

    return (ac_ivec4){ rounding_func(xf / scalarf),
                       rounding_func(yf / scalarf),
                       rounding_func(zf / scalarf),
                       rounding_func(wf / scalarf) };
}

int ac_ivec4_dot(const ac_ivec4* a, const ac_ivec4* b)
{
    if ( ac_ivec4_is_invalid(a) || ac_ivec4_is_invalid(b) )
    {
        return INT_INVALID;
    }

    return (a->x * b->x + a->y * b->y + a->z * b->z + a->w * b->w);
}
