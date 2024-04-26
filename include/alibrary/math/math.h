/**
 * \file
 * \brief Generic math functions and constants.
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \def AL_EPSILON
 * \brief An epsilon value for floating point comparisons against zero.
 */
#define AL_EPSILON 1e-6f

/**
 * \def AL_PI
 * \brief The value of pi.
 */
#define AL_PI 3.14159265358979323846f

/**
 * \brief Converts degrees to radians.
 * \param[in] degrees The angle in degrees.
 * \return The angle in radians.
 */
float al_deg_to_rad(float degrees);
/**
 * \brief Converts radians to degrees.
 * \param[in] radians The angle in radians.
 * \return The angle in degrees.
 */
float al_rad_to_deg(float radians);
/**
 * \brief Clamps a value between a minimum and maximum value.
 * \param[in] value The value to clamp.
 * \param[in] min The minimum value.
 * \param[in] max The maximum value.
 * \return The clamped value.
 */
float al_clamp(float value, float min, float max);

#ifdef __cplusplus
}
#endif
