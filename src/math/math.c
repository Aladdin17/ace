/**
 * \file
 * \brief Math functions implementation.
 */
#include <alibrary/math/math.h>
#define AL_DEG_RAD_SCALAR 180.0f

float al_deg_to_rad(float degrees)
{
    return degrees * AL_PI / AL_DEG_RAD_SCALAR;
}

float al_rad_to_deg(float radians)
{
    return radians * AL_DEG_RAD_SCALAR / AL_PI;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
float al_clamp(float value, float min, float max)
{
    float temp = value < min ? min : value;
    return temp > max ? max : temp;
}
