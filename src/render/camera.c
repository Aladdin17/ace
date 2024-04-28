/**
 * \file
 * \brief A camera class for rendering.
 */
#include <ace/render/camera.h>
#include <math.h>
#include <stdlib.h>

struct camera
{
    ac_vec3 position;
    float   yaw;
    float   pitch;
    ac_vec3 front;
    ac_vec3 right;
    ac_vec3 up;
    int     movement_direction;
    float   movement_step;
};

camera* camera_create(void)
{
    camera* cam = (camera*) malloc(sizeof(camera));
    camera_init(cam);
    return cam;
}

void camera_destroy(camera* cam)
{
    free(cam);
}

void camera_init(camera* cam)
{
    const float initial_yaw  = -90.0f;  // face towards negative z-axis
    const float initial_step = 0.5f;    // units

    cam->position           = (ac_vec3){ 0.0f, 0.0f, 0.0f };
    cam->up                 = (ac_vec3){ 0.0f, 1.0f, 0.0f };
    cam->right              = (ac_vec3){ 1.0f, 0.0f, 0.0f };
    cam->front              = (ac_vec3){ 0.0f, 0.0f, -1.0f };
    cam->yaw                = initial_yaw;
    cam->pitch              = 0.0f;
    cam->movement_direction = camera_direction_none;
    cam->movement_step      = initial_step;
    camera_update_orientation(cam);
}

void camera_update_orientation(camera* cam)
{
    // convert to radians
    const float pi          = 3.14159265359f;
    const float half_circle = 180.0f;
    float       yaw         = cam->yaw * pi / half_circle;
    float       pitch       = cam->pitch * pi / half_circle;

    // generate front vector
    ac_vec3 front;
    front.x = cosf(yaw) * cosf(pitch);
    front.y = sinf(pitch);
    front.z = sinf(yaw) * cosf(pitch);
    front   = ac_vec3_normalize(&front);

    // generate right and up vectors
    ac_vec3* up    = &cam->up;
    ac_vec3  right = ac_vec3_cross(&front, up);
    right          = ac_vec3_normalize(&right);

    // update camera
    cam->front = front;
    cam->right = right;
}

void camera_update_position(camera* cam)
{
    if ( cam->movement_direction == 0 )
    {
        return;
    }

    ac_vec3 movement = { 0.0f, 0.0f, 0.0f };

    if ( cam->movement_direction & camera_direction_forward )
    {
        movement = ac_vec3_add(&movement, &cam->front);
    }

    if ( cam->movement_direction & camera_direction_backward )
    {
        movement = ac_vec3_sub(&movement, &cam->front);
    }

    if ( cam->movement_direction & camera_direction_left )
    {
        movement = ac_vec3_sub(&movement, &cam->right);
    }

    if ( cam->movement_direction & camera_direction_right )
    {
        movement = ac_vec3_add(&movement, &cam->right);
    }

    if ( cam->movement_direction & camera_direction_up )
    {
        movement = ac_vec3_add(&movement, &cam->up);
    }

    if ( cam->movement_direction & camera_direction_down )
    {
        movement = ac_vec3_sub(&movement, &cam->up);
    }

    // apply resulting movement
    movement = ac_vec3_normalize(&movement);
    if ( !ac_vec3_is_nan(&movement) )
    {
        movement      = ac_vec3_scale(&movement, cam->movement_step);
        cam->position = ac_vec3_add(&cam->position, &movement);
    }
}

ac_vec3 camera_get_front(const camera* cam)
{
    return cam->front;
}

ac_vec3 camera_get_right(const camera* cam)
{
    return cam->right;
}

ac_vec3 camera_get_up(const camera* cam)
{
    return cam->up;
}

float camera_get_yaw(const camera* cam)
{
    return cam->yaw;
}

float camera_get_pitch(const camera* cam)
{
    return cam->pitch;
}

ac_vec3 camera_get_position(const camera* cam)
{
    return cam->position;
}

float camera_get_movement_step(const camera* cam)
{
    return cam->movement_step;
}

int camera_get_movement_direction(const camera* cam)
{
    return cam->movement_direction;
}

void camera_set_yaw(camera* cam, float yaw)
{
    const float max_angle = 180.0f;
    const float offset    = 360.0f;

    if ( yaw >= max_angle )
    {
        yaw -= offset;
    }
    else if ( yaw <= -max_angle )
    {
        yaw += offset;
    }

    cam->yaw = yaw;
}

void camera_set_pitch(camera* cam, float pitch)
{
    const float max_angle = 89.0f;

    if ( pitch >= max_angle )
    {
        pitch = max_angle;
    }
    else if ( pitch < -max_angle )
    {
        pitch = -max_angle;
    }

    cam->pitch = pitch;
}

void camera_set_position(camera* cam, ac_vec3 position)
{
    cam->position = position;
}

void camera_set_movement_step(camera* cam, float step)
{
    cam->movement_step = step;
}

void camera_set_movement_direction(camera* cam, int direction)
{
    cam->movement_direction = direction;
}
