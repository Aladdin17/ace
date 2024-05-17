/**
 * \file
 * \author Christien Alden
 * \brief 3-component vector types and functions implementation.
 */
#include <ace/math/math.h>
#include <ace/math/vec2.h>
#include <math.h>

//--------------------------------------------------------------------------------------------------
// float
//--------------------------------------------------------------------------------------------------

ac_vec2 ac_vec2_zero(void)
{
    return (ac_vec2){ 0.0f, 0.0f };
}

ac_vec2 ac_vec2_nan(void)
{
    return (ac_vec2){ NAN, NAN };
}

bool ac_vec2_is_zero(const ac_vec2* v)
{
    return ac_vec2_magnitude(v) <= AC_EPSILON;
}

// NOLINTNEXTLINE(readability-function-cognitive-complexity)
bool ac_vec2_is_nan(const ac_vec2* v)
{
    return (isnan(v->x) || isnan(v->y));
}

bool ac_vec2_is_equal(const ac_vec2* a, const ac_vec2* b)
{
    ac_vec2 diff = ac_vec2_sub(a, b);
    return ac_vec2_is_zero(&diff);
}

ac_vec2 ac_vec2_add(const ac_vec2* a, const ac_vec2* b)
{
    return (ac_vec2){ a->x + b->x, a->y + b->y };
}

ac_vec2 ac_vec2_sub(const ac_vec2* a, const ac_vec2* b)
{
    return (ac_vec2){ a->x - b->x, a->y - b->y };
}

ac_vec2 ac_vec2_negate(const ac_vec2* v)
{
    return (ac_vec2){ -v->x, -v->y };
}

ac_vec2 ac_vec2_scale(const ac_vec2* v, float scalar)
{
    return (ac_vec2){ v->x * scalar, v->y * scalar };
}

float ac_vec2_dot(const ac_vec2* a, const ac_vec2* b)
{
    return (a->x * b->x + a->y * b->y);
}

float ac_vec2_magnitude(const ac_vec2* v)
{
    return sqrtf(ac_vec2_dot(v, v));
}

ac_vec2 ac_vec2_normalize(const ac_vec2* v)
{
    float magnitude = ac_vec2_magnitude(v);
    if ( magnitude <= AC_EPSILON )
    {
        return ac_vec2_nan();
    }

    // calculate the inverse magnitude and scale the vector
    float inv_magnitude = 1.0f / magnitude;
    return ac_vec2_scale(v, inv_magnitude);
}

//--------------------------------------------------------------------------------------------------
// int
//--------------------------------------------------------------------------------------------------

ac_ivec2 ac_ivec2_zero(void)
{
    return (ac_ivec2){ 0, 0 };
}

ac_ivec2 ac_ivec2_invalid(void)
{
    return (ac_ivec2){ INT_INVALID, INT_INVALID };
}

bool ac_ivec2_is_zero(const ac_ivec2* v)
{
    return (v->x == 0 && v->y == 0);
}

bool ac_ivec2_is_invalid(const ac_ivec2* v)
{
    return (v->x == INT_INVALID || v->y == INT_INVALID);
}

bool ac_ivec2_is_equal(const ac_ivec2* a, const ac_ivec2* b)
{
    if ( ac_ivec2_is_invalid(a) || ac_ivec2_is_invalid(b) )
    {
        return false;
    }

    return (a->x == b->x && a->y == b->y);
}

ac_ivec2 ac_ivec2_add(const ac_ivec2* a, const ac_ivec2* b)
{
    if ( ac_ivec2_is_invalid(a) || ac_ivec2_is_invalid(b) )
    {
        return ac_ivec2_invalid();
    }

    return (ac_ivec2){ a->x + b->x, a->y + b->y };
}

ac_ivec2 ac_ivec2_sub(const ac_ivec2* a, const ac_ivec2* b)
{
    if ( ac_ivec2_is_invalid(a) || ac_ivec2_is_invalid(b) )
    {
        return ac_ivec2_invalid();
    }

    return (ac_ivec2){ a->x - b->x, a->y - b->y };
}

ac_ivec2 ac_ivec2_negate(const ac_ivec2* v)
{
    if ( ac_ivec2_is_invalid(v) )
    {
        return ac_ivec2_invalid();
    }

    return (ac_ivec2){ -v->x, -v->y };
}

ac_ivec2 ac_ivec2_scale(const ac_ivec2* v, int scalar)
{
    if ( ac_ivec2_is_invalid(v) )
    {
        return ac_ivec2_invalid();
    }

    return (ac_ivec2){ v->x * scalar, v->y * scalar };
}

ac_ivec2 ac_ivec2_divide(const ac_ivec2* v, int scalar)
{
    if ( ac_ivec2_is_invalid(v) || scalar == 0 )
    {
        return ac_ivec2_invalid();
    }

    return (ac_ivec2){ v->x / scalar, v->y / scalar };
}

ac_ivec2 ac_ivec2_divide_ext(const ac_ivec2* v, int scalar, int (*rounding_func)(float))
{
    if ( ac_ivec2_is_invalid(v) || scalar == 0 )
    {
        return ac_ivec2_invalid();
    }

    // convert to float
    float scalarf = (float) scalar;
    float xf      = (float) v->x;
    float yf      = (float) v->y;

    // divide and round
    return (ac_ivec2){ rounding_func(xf / scalarf), rounding_func(yf / scalarf) };
}

int ac_ivec2_dot(const ac_ivec2* a, const ac_ivec2* b)
{
    if ( ac_ivec2_is_invalid(a) || ac_ivec2_is_invalid(b) )
    {
        return INT_INVALID;
    }

    return (a->x * b->x + a->y * b->y);
}
