/**
 * \file
 * \brief A camera class for rendering.
 */
#pragma once
#include "../math/vec3.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup camera camera
 * \brief A camera class for rendering.
 */

/**
 * \ingroup camera
 * \enum ac_camera_direction
 * \brief The movement directions of the camera.
 * \details
 * The movement directions of the camera are a bitfield. The camera can move in multiple
 * directions at once. The directions are combined using the bitwise OR operator '|'. To unset a
 * direction use the bitwise AND operator '&' along with the bitwise NOT '~' operator. To unset all
 * directions set the bitfield to ac_camera_direction_none.
 * \see ac_camera_set_movement_direction
 * \see ac_camera_get_movement_direction
 */
typedef enum ac_camera_direction
{
    ac_camera_direction_none     = 0,
    ac_camera_direction_forward  = 1,
    ac_camera_direction_backward = 1 << 1,
    ac_camera_direction_left     = 1 << 2,
    ac_camera_direction_right    = 1 << 3,
    ac_camera_direction_up       = 1 << 4,
    ac_camera_direction_down     = 1 << 5
} ac_camera_direction;

/**
 * \ingroup camera
 * \struct camera
 * \brief A basic camera structure.
 * \details
 * The camera can be moved in 3D space and rotated. The camera has a position, a front vector, a
 * right vector, an up vector, a yaw, a pitch, and a movement step. The camera can move in multiple
 * directions at once. The camera can be moved forward, backward, left, right, up, and down. The
 * camera can also be rotated around the yaw and pitch axes.
 */
typedef struct ac_camera ac_camera;

/**
 * \ingroup camera
 * \brief Create a new camera.
 * \return A pointer to the camera.
 */
ac_camera* ac_camera_create(void);
/**
 * \ingroup camera
 * \brief Destroy a camera.
 * \param[out] cam The camera to destroy.
 */
void       ac_camera_destroy(ac_camera* cam);
/**
 * \ingroup camera
 * \brief Initialize a camera with default values.
 * \param[out] cam The camera to initialize.
 */
void       ac_camera_init(ac_camera* cam);
/**
 * \ingroup camera
 * \brief Update the orientation of the camera.
 * \param[in,out] cam The camera to update.
 */
void       ac_camera_update_orientation(ac_camera* cam);
/**
 * \ingroup camera
 * \brief Update the position of the camera.
 * \param[in,out] cam The camera to update.
 */
void       ac_camera_update_position(ac_camera* cam);
/**
 * \ingroup camera
 * \brief Get the front vector of the camera.
 * \param[in] cam The camera.
 * \return The front vector.
 */
ac_vec3    ac_camera_get_front(const ac_camera* cam);
/**
 * \ingroup camera
 * \brief Get the right vector of the camera.
 * \param[in] cam The camera.
 * \return The right vector.
 */
ac_vec3    ac_camera_get_right(const ac_camera* cam);
/**
 * \ingroup camera
 * \brief Get the up vector of the camera.
 * \param[in] cam The camera.
 * \return The up vector.
 */
ac_vec3    ac_camera_get_up(const ac_camera* cam);
/**
 * \ingroup camera
 * \brief Get the yaw of the camera.
 * \param[in] cam The camera.
 * \return The yaw.
 */
float      ac_camera_get_yaw(const ac_camera* cam);
/**
 * \ingroup camera
 * \brief Get the pitch of the camera.
 * \param[in] cam The camera.
 * \return The pitch.
 */
float      ac_camera_get_pitch(const ac_camera* cam);
/**
 * \ingroup camera
 * \brief Get the position of the camera.
 * \param[in] cam The camera.
 * \return The position.
 */
ac_vec3    ac_camera_get_position(const ac_camera* cam);
/**
 * \ingroup camera
 * \brief Get the movement speed of the camera.
 * \param[in] cam The camera.
 * \return The movement step.
 */
float      ac_camera_get_movement_step(const ac_camera* cam);
/**
 * \ingroup camera
 * \brief Returns the movement direction of the camera.
 * \param[in] cam The camera.
 * \return The movement direction bitfield.
 */
int        ac_camera_get_movement_direction(const ac_camera* cam);
/**
 * \ingroup camera
 * \brief Set the yaw of the camera.
 * \param[in,out] cam The camera.
 * \param[in] yaw The yaw.
 * \note The yaw is clamped between 0 and 360 degrees.
 */
void       ac_camera_set_yaw(ac_camera* cam, float yaw);
/**
 * \ingroup camera
 * \brief Set the pitch of the camera.
 * \param[in,out] cam The camera.
 * \param[in] pitch The pitch.
 * \note The pitch is clamped between -89 and 89 degrees.
 */
void       ac_camera_set_pitch(ac_camera* cam, float pitch);
/**
 * \ingroup camera
 * \brief Set the position of the camera.
 * \param[in,out] cam The camera.
 * \param[in] position The position.
 */
void       ac_camera_set_position(ac_camera* cam, ac_vec3 position);
/**
 * \ingroup camera
 * \brief Set the movement speed of the camera.
 * \param[in,out] cam The camera.
 * \param[in] step The movement step.
 */
void       ac_camera_set_movement_step(ac_camera* cam, float step);
/**
 * \ingroup camera
 * \brief Set the movement direction of the camera.
 * \param[in,out] cam The camera.
 * \param[in] direction The movement direction bitfield.
 * \see ac_camera_direction.
 * \details
 * The direction is a bitfield of ac_camera_direction. The camera will move in the direction of the
 * bitfield. The camera will continue to move in a given direction until that bit is unset. To set
 * the bit use the bitwise OR operator '|', to unset the bit use the bitwise AND operator '&' along
 * with the bitwise NOT '~' operator. For example, to set the forward and right bits:
 * ac_camera_direction_forward | ac_camera_direction_right.
 * To unset the forward bit: direction & ~ac_camera_direction_forward
 * You can also unset all bits by setting the direction to ac_camera_direction_none.
 */
void       ac_camera_set_movement_direction(ac_camera* cam, int direction);

#ifdef __cplusplus
}
#endif
