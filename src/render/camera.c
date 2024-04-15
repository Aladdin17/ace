/**
 * \file
 * \brief A camera class for rendering.
 */
#include <alibrary/render/camera.h>
#include <math.h>
#include <stdlib.h>

struct Camera
{
    Vec3  position;
    float yaw;
    float pitch;
    Vec3  front;
    Vec3  right;
    Vec3  up;
    int   movementDirection;
    float movementStep;
};

Camera* camera_create(void)
{
    return (Camera*) malloc(sizeof(Camera));
}

void camera_destroy(Camera* camera)
{
    free(camera);
}

void camera_init(Camera* camera)
{
    const float initialYaw  = -90.0f;  // face towards negative z-axis
    const float initialStep = 2.5f;    // units

    camera->position          = (Vec3){ 0.0f, 0.0f, 0.0f };
    camera->yaw               = initialYaw;
    camera->pitch             = 0.0f;
    camera->movementDirection = CameraDirection_NONE;
    camera->movementStep      = initialStep;
    camera_update_orientation(camera);
}

void camera_update_orientation(Camera* camera)
{
    // generate front vector
    Vec3 front;
    front.x = cosf(camera->yaw) * cosf(camera->pitch);
    front.y = sinf(camera->pitch);
    front.z = sinf(camera->yaw) * cosf(camera->pitch);
    front   = Vec3Normalize(&front);

    // generate right and up vectors
    Vec3 up    = { 0.0f, 1.0f, 0.0f };
    Vec3 right = Vec3Cross(&front, &up);
    right      = Vec3Normalize(&right);
}

void camera_update_position(Camera* camera)
{
    if ( camera->movementDirection == 0 )
    {
        return;
    }

    if ( camera->movementDirection & CameraDirection_FORWARD )
    {
        Vec3 step        = Vec3Scale(&camera->front, camera->movementStep);
        camera->position = Vec3Add(&camera->position, &step);
    }

    if ( camera->movementDirection & CameraDirection_BACKWARD )
    {
        Vec3 step        = Vec3Scale(&camera->front, camera->movementStep);
        camera->position = Vec3Sub(&camera->position, &step);
    }

    if ( camera->movementDirection & CameraDirection_LEFT )
    {
        Vec3 step        = Vec3Scale(&camera->right, camera->movementStep);
        camera->position = Vec3Sub(&camera->position, &step);
    }

    if ( camera->movementDirection & CameraDirection_RIGHT )
    {
        Vec3 step        = Vec3Scale(&camera->right, camera->movementStep);
        camera->position = Vec3Add(&camera->position, &step);
    }

    if ( camera->movementDirection & CameraDirection_UP )
    {
        Vec3 step        = Vec3Scale(&camera->up, camera->movementStep);
        camera->position = Vec3Add(&camera->position, &step);
    }

    if ( camera->movementDirection & CameraDirection_DOWN )
    {
        Vec3 step        = Vec3Scale(&camera->up, camera->movementStep);
        camera->position = Vec3Sub(&camera->position, &step);
    }
}

Vec3 camera_get_front(const Camera* camera)
{
    return camera->front;
}

Vec3 camera_get_right(const Camera* camera)
{
    return camera->right;
}

Vec3 camera_get_up(const Camera* camera)
{
    return camera->up;
}

float camera_get_yaw(const Camera* camera)
{
    return camera->yaw;
}

float camera_get_pitch(const Camera* camera)
{
    return camera->pitch;
}

Vec3 camera_get_position(const Camera* camera)
{
    return camera->position;
}

float camera_get_movement_step(const Camera* camera)
{
    return camera->movementStep;
}

void camera_set_yaw(Camera* camera, float yaw)
{
    const float maxAngle = 180.0f;

    if ( yaw >= maxAngle )
    {
        yaw -= maxAngle;
    }
    else if ( yaw < maxAngle )
    {
        yaw += maxAngle;
    }

    camera->yaw = yaw;
}

void camera_set_pitch(Camera* camera, float pitch)
{
    const float maxAngle = 89.0f;

    if ( pitch >= maxAngle )
    {
        pitch = maxAngle;
    }
    else if ( pitch <= -maxAngle )
    {
        pitch = -maxAngle;
    }

    camera->pitch = pitch;
}

void camera_set_position(Camera* camera, Vec3 position)
{
    camera->position = position;
}

void camera_set_movement_step(Camera* camera, float step)
{
    camera->movementStep = step;
}

void camera_set_movement_direction(Camera* camera, CameraDirection direction)
{
    camera->movementDirection = direction;
}
