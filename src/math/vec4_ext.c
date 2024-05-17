#include <ace/math/math.h>
#include <ace/math/vec4_ext.h>
#include <math.h>

float ac_vec4_angle(const ac_vec4* a, const ac_vec4* b)
{
    // guard against NaN vectors
    if ( ac_vec4_is_nan(a) || ac_vec4_is_nan(b) )
    {
        return NAN;
    }

    // guard against zero vectors
    if ( ac_vec4_is_zero(a) || ac_vec4_is_zero(b) )
    {
        return NAN;
    }

    // the dot product can be defined as: dot = |a| * |b| * cos(theta)
    // therefore, theta = acos(dot / (|a| * |b|))
    float dot  = ac_vec4_dot(a, b);
    float magA = ac_vec4_magnitude(a);
    float magB = ac_vec4_magnitude(b);
    return acosf(dot / (magA * magB));
}

float ac_vec4_distance(const ac_vec4* a, const ac_vec4* b)
{
    ac_vec4 diff = ac_vec4_sub(a, b);
    return ac_vec4_magnitude(&diff);
}

ac_vec4 ac_vec4_lerp(const ac_vec4* a, const ac_vec4* b, float interpolation_factor)
{
    // clamp the interpolation_factor between 0 and 1
    interpolation_factor = ac_clamp(interpolation_factor, 0.0f, 1.0f);

    // calculate the linear interpolation
    ac_vec4 diff  = ac_vec4_sub(b, a);
    ac_vec4 delta = ac_vec4_scale(&diff, interpolation_factor);
    return ac_vec4_add(a, &delta);
}

ac_vec4 ac_vec4_project(const ac_vec4* a, const ac_vec4* b)
{
    // guard against NaN vectors
    if ( ac_vec4_is_nan(a) || ac_vec4_is_nan(b) )
    {
        return ac_vec4_nan();
    }

    // guard against zero vectors
    if ( ac_vec4_is_zero(a) || ac_vec4_is_zero(b) )
    {
        return ac_vec4_nan();
    }

    // the projection can be derived by calculating the dot product of a and b
    // and then scaling b by the dot product divided by the magnitude of b squared
    // projection = (a . b) / |b|^2 * b
    float dot                       = ac_vec4_dot(a, b);
    float mag_b                     = ac_vec4_magnitude(b);
    float inverse_magnitude_squared = 1.0f / (mag_b * mag_b);
    return ac_vec4_scale(b, dot * inverse_magnitude_squared);
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
ac_vec4 ac_vec4_reflect(const ac_vec4* incoming, const ac_vec4* normal)
{
    // to reflect a vector v about a normal n, we want to project v onto n
    // and then subtract the projection from v twice, which is the same as
    // subtracting 2 times the projection from v. This is equivalent to:
    // reflect = v - 2 * (v . n) * n

    // check incoming vector for validity and return a NaN vector if it is invalid
    if ( ac_vec4_is_nan(incoming) || ac_vec4_is_zero(incoming) )
    {
        return ac_vec4_nan();
    }
    // normalize the normal to ensure the projection is correct and
    // return a NaN vector if the normal is zero
    ac_vec4 n_normalized = ac_vec4_normalize(normal);
    if ( ac_vec4_is_nan(&n_normalized) )
    {
        return ac_vec4_nan();
    }

    // the projection modifier is used to scale the projection of v onto n
    static const float projection_modifier = 2.0f;

    // calculate the projection of v onto n
    float   projection = ac_vec4_dot(incoming, &n_normalized);
    ac_vec4 scaled_n   = ac_vec4_scale(&n_normalized, projection_modifier * projection);
    return ac_vec4_sub(incoming, &scaled_n);
}

ac_vec4 ac_vec4_mult_matrix(const ac_vec4* vec, const ac_mat4* mat)
{
    // guard against NaN vectors
    if ( ac_vec4_is_nan(vec) || ac_mat4_is_nan(mat) )
    {
        return ac_vec4_nan();
    }

    // multiply the vector by the matrix
    return (ac_vec4){ .x = vec->x * mat->a + vec->y * mat->e + vec->z * mat->i + vec->w * mat->m,
                      .y = vec->x * mat->b + vec->y * mat->f + vec->z * mat->j + vec->w * mat->n,
                      .z = vec->x * mat->c + vec->y * mat->g + vec->z * mat->k + vec->w * mat->o,
                      .w = vec->x * mat->d + vec->y * mat->h + vec->z * mat->l + vec->w * mat->p };
}
