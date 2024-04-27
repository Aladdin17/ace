#include <alibrary/math/math.h>
#include <alibrary/math/vec3_ext.h>
#include <math.h>

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
