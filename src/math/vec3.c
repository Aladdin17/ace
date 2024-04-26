/**
 * \file
 * \author Christien Alden
 * \brief 3-component vector types and functions implementation.
 */
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

//--------------------------------------------------------------------------------------------------
// Utility functions
//--------------------------------------------------------------------------------------------------

float vec3_angle(const vec3* a, const vec3* b)
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
vec3 vec3_reflect(const vec3* incoming, const vec3* normal)
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
    vec3 n_normalized = vec3_normalize(normal);
    if ( vec3_is_nan(&n_normalized) )
    {
        return vec3_nan();
    }

    // the projection modifier is used to scale the projection of v onto n
    static const float projection_modifier = 2.0f;

    // calculate the projection of v onto n
    float projection = vec3_dot(incoming, &n_normalized);
    vec3  scaled_n   = vec3_scale(&n_normalized, projection_modifier * projection);
    return vec3_sub(incoming, &scaled_n);
}

float vec3_distance(const vec3* a, const vec3* b)
{
    vec3 diff = vec3_sub(a, b);
    return vec3_magnitude(&diff);
}

vec3 vec3_lerp(const vec3* a, const vec3* b, float interpolation_factor)
{
    // clamp the interpolation_factor between 0 and 1
    interpolation_factor = al_clamp(interpolation_factor, 0.0f, 1.0f);

    // calculate the linear interpolation
    vec3 diff  = vec3_sub(b, a);
    vec3 delta = vec3_scale(&diff, interpolation_factor);
    return vec3_add(a, &delta);
}

vec3 vec3_project(const vec3* a, const vec3* b)
{
    // guard against NaN vectors
    if ( vec3_is_nan(a) || vec3_is_nan(b) )
    {
        return vec3_nan();
    }

    // guard against zero vectors
    if ( vec3_is_zero(a) || vec3_is_zero(b) )
    {
        return vec3_nan();
    }

    // the projection can be derived by calculating the dot product of a and b
    // and then scaling b by the dot product divided by the magnitude of b squared
    // projection = (a . b) / |b|^2 * b
    float dot                       = vec3_dot(a, b);
    float mag_b                     = vec3_magnitude(b);
    float inverse_magnitude_squared = 1.0f / (mag_b * mag_b);
    return vec3_scale(b, dot * inverse_magnitude_squared);
}
