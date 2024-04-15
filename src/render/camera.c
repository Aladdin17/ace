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
    Camera* camera = (Camera*) malloc(sizeof(Camera));
    camera_init(camera);
    return camera;
}

void camera_destroy(Camera* camera)
{
    free(camera);
}

void camera_init(Camera* camera)
{
    const float initialYaw  = -90.0f;  // face towards negative z-axis
    const float initialStep = 0.5f;    // units

    camera->position          = (Vec3){ 0.0f, 0.0f, 0.0f };
    camera->up                = (Vec3){ 0.0f, 1.0f, 0.0f };
    camera->right             = (Vec3){ 1.0f, 0.0f, 0.0f };
    camera->front             = (Vec3){ 0.0f, 0.0f, -1.0f };
    camera->yaw               = initialYaw;
    camera->pitch             = 0.0f;
    camera->movementDirection = CameraDirection_NONE;
    camera->movementStep      = initialStep;
    camera_update_orientation(camera);
}

void camera_update_orientation(Camera* camera)
{
    // convert to radians
    const float pi          = 3.14159265359f;
    const float half_circle = 180.0f;
    float       yaw         = camera->yaw * pi / half_circle;
    float       pitch       = camera->pitch * pi / half_circle;

    // generate front vector
    Vec3 front;
    front.x = cosf(yaw) * cosf(pitch);
    front.y = sinf(pitch);
    front.z = sinf(yaw) * cosf(pitch);
    front   = Vec3Normalize(&front);

    // generate right and up vectors
    Vec3* up    = &camera->up;
    Vec3  right = Vec3Cross(&front, up);
    right       = Vec3Normalize(&right);

    // update camera
    camera->front = front;
    camera->right = right;
}

void camera_update_position(Camera* camera)
{
    if ( camera->movementDirection == 0 )
    {
        return;
    }

    Vec3 movement = { 0.0f, 0.0f, 0.0f };

    if ( camera->movementDirection & CameraDirection_FORWARD )
    {
        movement = Vec3Add(&movement, &camera->front);
    }

    if ( camera->movementDirection & CameraDirection_BACKWARD )
    {
        movement = Vec3Sub(&movement, &camera->front);
    }

    if ( camera->movementDirection & CameraDirection_LEFT )
    {
        movement = Vec3Sub(&movement, &camera->right);
    }

    if ( camera->movementDirection & CameraDirection_RIGHT )
    {
        movement = Vec3Add(&movement, &camera->right);
    }

    if ( camera->movementDirection & CameraDirection_UP )
    {
        movement = Vec3Add(&movement, &camera->up);
    }

    if ( camera->movementDirection & CameraDirection_DOWN )
    {
        movement = Vec3Sub(&movement, &camera->up);
    }

    // apply resulting movement
    movement = Vec3Normalize(&movement);
    if ( !Vec3IsNaN(&movement) )
    {
        movement         = Vec3Scale(&movement, camera->movementStep);
        camera->position = Vec3Add(&camera->position, &movement);
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
    const float offset   = 360.0f;

    if ( yaw >= maxAngle )
    {
        yaw -= offset;
    }
    else if ( yaw <= -maxAngle )
    {
        yaw += offset;
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
    else if ( pitch < -maxAngle )
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

void camera_set_movement_direction(Camera* camera, int direction)
{
    camera->movementDirection = direction;
}

int camera_get_movement_direction(Camera* camera)
{
    return camera->movementDirection;
}
