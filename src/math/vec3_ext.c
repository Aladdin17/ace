#include <ace/math/math.h>
#include <ace/math/vec3_ext.h>
#include <math.h>

float ac_vec3_angle(const ac_vec3* a, const ac_vec3* b)
{
    // guard against NaN vectors
    if ( ac_vec3_is_nan(a) || ac_vec3_is_nan(b) )
    {
        return NAN;
    }

    // guard against zero vectors
    if ( ac_vec3_is_zero(a) || ac_vec3_is_zero(b) )
    {
        return NAN;
    }

    // the dot product can be defined as: dot = |a| * |b| * cos(theta)
    // therefore, theta = acos(dot / (|a| * |b|))
    float dot  = ac_vec3_dot(a, b);
    float magA = ac_vec3_magnitude(a);
    float magB = ac_vec3_magnitude(b);
    return acosf(dot / (magA * magB));
}

float vec3_distance(const ac_vec3* a, const ac_vec3* b)
{
    ac_vec3 diff = ac_vec3_sub(a, b);
    return ac_vec3_magnitude(&diff);
}

ac_vec3 vec3_lerp(const ac_vec3* a, const ac_vec3* b, float interpolation_factor)
{
    // clamp the interpolation_factor between 0 and 1
    interpolation_factor = ac_clamp(interpolation_factor, 0.0f, 1.0f);

    // calculate the linear interpolation
    ac_vec3 diff  = ac_vec3_sub(b, a);
    ac_vec3 delta = ac_vec3_scale(&diff, interpolation_factor);
    return ac_vec3_add(a, &delta);
}

ac_vec3 vec3_project(const ac_vec3* a, const ac_vec3* b)
{
    // guard against NaN vectors
    if ( ac_vec3_is_nan(a) || ac_vec3_is_nan(b) )
    {
        return ac_vec3_nan();
    }

    // guard against zero vectors
    if ( ac_vec3_is_zero(a) || ac_vec3_is_zero(b) )
    {
        return ac_vec3_nan();
    }

    // the projection can be derived by calculating the dot product of a and b
    // and then scaling b by the dot product divided by the magnitude of b squared
    // projection = (a . b) / |b|^2 * b
    float dot                       = ac_vec3_dot(a, b);
    float mag_b                     = ac_vec3_magnitude(b);
    float inverse_magnitude_squared = 1.0f / (mag_b * mag_b);
    return ac_vec3_scale(b, dot * inverse_magnitude_squared);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
ac_vec3 vec3_reflect(const ac_vec3* incoming, const ac_vec3* normal)
{
    // to reflect a vector v about a normal n, we want to project v onto n
    // and then subtract the projection from v twice, which is the same as
    // subtracting 2 times the projection from v. This is equivalent to:
    // reflect = v - 2 * (v . n) * n

    // check incoming vector for validity and return a NaN vector if it is invalid
    if ( ac_vec3_is_nan(incoming) || ac_vec3_is_zero(incoming) )
    {
        return ac_vec3_nan();
    }
    // normalize the normal to ensure the projection is correct and
    // return a NaN vector if the normal is zero
    ac_vec3 n_normalized = ac_vec3_normalize(normal);
    if ( ac_vec3_is_nan(&n_normalized) )
    {
        return ac_vec3_nan();
    }

    // the projection modifier is used to scale the projection of v onto n
    static const float projection_modifier = 2.0f;

    // calculate the projection of v onto n
    float   projection = ac_vec3_dot(incoming, &n_normalized);
    ac_vec3 scaled_n   = ac_vec3_scale(&n_normalized, projection_modifier * projection);
    return ac_vec3_sub(incoming, &scaled_n);
}

ac_vec3 ac_vec3_mult_matrix(const ac_vec3* vec, const ac_mat3* mat)
{
    // guard against NaN vectors
    if ( ac_vec3_is_nan(vec) || ac_mat3_is_nan(mat) )
    {
        return ac_vec3_nan();
    }

    // multiply the vector by the matrix
    // this is wrong... fix it
    return (ac_vec3){ .x = vec->x * mat->a + vec->y * mat->d + vec->z * mat->g,
                      .y = vec->x * mat->b + vec->y * mat->e + vec->z * mat->h,
                      .z = vec->x * mat->c + vec->y * mat->f + vec->z * mat->i };
}
