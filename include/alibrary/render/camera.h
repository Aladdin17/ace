/**
 * \file
 * \brief A camera class for rendering.
 */
#pragma once
#include "../math/vec3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum CameraDirection
{
    CameraDirection_NONE     = 0,
    CameraDirection_FORWARD  = 1,
    CameraDirection_BACKWARD = 1 << 1,
    CameraDirection_LEFT     = 1 << 2,
    CameraDirection_RIGHT    = 1 << 3,
    CameraDirection_UP       = 1 << 4,
    CameraDirection_DOWN     = 1 << 5
} CameraDirection;

// opaque structure
typedef struct Camera Camera;

/**
 * \brief Create a new camera.
 * \return A pointer to the camera.
 */
Camera* camera_create(void);
/**
 * \brief Destroy a camera.
 * \param camera The camera to destroy.
 */
void    camera_destroy(Camera* camera);
/**
 * \brief Initialize a camera with default values.
 * \param camera The camera to initialize.
 */
void    camera_init(Camera* camera);
/**
 * \brief Update the orientation of the camera.
 * \param camera The camera to update.
 */
void    camera_update_orientation(Camera* camera);
/**
 * \brief Update the position of the camera.
 * \param camera The camera to update.
 */
void    camera_update_position(Camera* camera);

/**
 * \brief Get the front vector of the camera.
 * \param camera The camera.
 * \return The front vector.
 */
Vec3 camera_get_front(const Camera* camera);
/**
 * \brief Get the right vector of the camera.
 * \param camera The camera.
 * \return The right vector.
 */
Vec3 camera_get_right(const Camera* camera);
/**
 * \brief Get the up vector of the camera.
 * \param camera The camera.
 * \return The up vector.
 */
Vec3 camera_get_up(const Camera* camera);

/**
 * \brief Get the yaw of the camera.
 * \param camera The camera.
 * \return The yaw.
 */
float camera_get_yaw(const Camera* camera);
/**
 * \brief Get the pitch of the camera.
 * \param camera The camera.
 * \return The pitch.
 */
float camera_get_pitch(const Camera* camera);
/**
 * \brief Get the position of the camera.
 * \param camera The camera.
 * \return The position.
 */
Vec3  camera_get_position(const Camera* camera);
/**
 * \brief Get the movement speed of the camera.
 * \param camera The camera.
 * \return The movement step.
 */
float camera_get_movement_step(const Camera* camera);

/**
 * \brief Set the yaw of the camera.
 * \param camera The camera.
 * \param yaw The yaw.
 * \note The yaw is clamped between 0 and 360 degrees.
 */
void camera_set_yaw(Camera* camera, float yaw);
/**
 * \brief Set the pitch of the camera.
 * \param camera The camera.
 * \param pitch The pitch.
 * \note The pitch is clamped between -89 and 89 degrees.
 */
void camera_set_pitch(Camera* camera, float pitch);
/**
 * \brief Set the position of the camera.
 * \param camera The camera.
 * \param position The position.
 */
void camera_set_position(Camera* camera, Vec3 position);
/**
 * \brief Set the movement speed of the camera.
 * \param camera The camera.
 * \param speed The movement step.
 */
void camera_set_movement_step(Camera* camera, float step);

/**
 * \brief Set the movement direction of the camera.
 * \param camera The camera.
 * \param direction The movement direction.
 * \note The direction is a bitfield of CameraDirection.
 * \see CameraDirection
 *
 * \details
 * The direction is a bitfield of CameraDirection. The camera will move in the direction of the
 * bitfield. The camera will continue to move in a given direction until that bit is unset. To set
 * the bit use the bitwise OR operator '|', to unset the bit use the bitwise AND operator '&' along
 * with the bitwise NOT '~' operator. For example, to set the forward and right bits:
 * CameraDirection_FORWARD | CameraDirection_RIGHT.
 * To unset the forward bit: direction & ~CameraDirection_FORWARD
 * You can also unset all bits by setting the direction to CameraDirection_NONE.
 */
void camera_set_movement_direction(Camera* camera, CameraDirection direction);

#ifdef __cplusplus
}
#endif
