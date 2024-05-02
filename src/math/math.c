/**
 * \file
 * \brief Math functions implementation.
 */
#include <ace/math/math.h>
#include <math.h>
#define AC_DEG_RAD_SCALAR 180.0f

float ac_deg_to_rad(float degrees)
{
    return degrees * AC_PI / AC_DEG_RAD_SCALAR;
}

float ac_rad_to_deg(float radians)
{
    return radians * AC_DEG_RAD_SCALAR / AC_PI;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
float ac_clamp(float value, float min, float max)
{
    float temp = value < min ? min : value;
    return temp > max ? max : temp;
}

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
bool ac_float_compare(float a, float b, float epsilon)
{
    return fabsf(a - b) <= epsilon;
}
