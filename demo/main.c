
#include "GL/freeglut.h"

#include <stdio.h>

#include <ace/math/vec3.h>
#include <ace/physics/phys_world.h>
#include <ace/geometry/shapes.h>
#include <ace/render/camera.h>
#include <math.h>
#include <ace/math/math.h>

typedef struct
{
    int update_rate;
    int last_frame_time;
    int current_frame_time;
} frame_time;

typedef struct
{
    float radius;
    float pitch_angle;
    float yaw_angle;
    ac_vec3 target;
} orbit_camera;

typedef struct
{
    unsigned target;
    float pitch;
    float yaw;
    float power;
    bool strike;
} cue_stick;

// camera
PhysWorld physicsWorld;
frame_time time;
orbit_camera camera;
cue_stick stick;

// entities
unsigned cueBallID;
unsigned ballIDs[10];
Collider ballCollider;
float ballRadius = 1; // used for all the balls so their size is uniform
ac_vec3 default_cue_position = {0, 2, 15};

// table

unsigned tableTopID;
ac_vec3 tableTopPos = {0, 0, 0};
Collider tableTopCollider;
ac_vec3 tableTopHalfExtents = {10, 1, 20};

// functions
void collisionCallback(unsigned e1, unsigned e2);

void update_view_matrix(const orbit_camera* cam)
{
    float pitch_rad = ac_deg_to_rad(cam->pitch_angle);
    float yaw_rad = ac_deg_to_rad(cam->yaw_angle);

    ac_vec3 up = (ac_vec3){0, 1, 0};
    ac_vec3 radial = (ac_vec3){
        .x = cosf(yaw_rad) * cosf(pitch_rad),
        .y = sinf(pitch_rad),
        .z = sinf(yaw_rad) * cosf(pitch_rad)
    };
    radial = ac_vec3_normalize(&radial);
    radial = ac_vec3_scale(&radial, cam->radius);
    ac_vec3 eye = ac_vec3_add(&cam->target, &radial);
    ac_vec3 look_at = cam->target;

    gluLookAt(
        eye.x, eye.y, eye.z,
        look_at.x, look_at.y, look_at.z,
        up.x, up.y, up.z
    );
}

void render_scene()
{
    // draw balls
    for (int i = 0; i < 10; i++)
    {
        glPushMatrix();
        ac_vec3* pos = &physicsWorld.positions[ballIDs[i]];
        glTranslatef(pos->x, pos->y, pos->z);
        glColor3f(0.4f, 0.1f, 0.1f);
        glutSolidSphere(1.0, 20, 20);
        glPopMatrix();
    }

    // draw cue ball
    glPushMatrix();
        ac_vec3* pos = &physicsWorld.positions[cueBallID];
        glTranslatef(pos->x, pos->y, pos->z);
        glColor3f(0.8f, 0.8f, 0.8f);
        glutSolidSphere(1, 20, 20);

        // draw cue stick, if cue ball stopped
        glPushMatrix();
            glColor3f(0.651f, 0.51f, 0.035f); // brown
            glRotatef(stick.yaw, 0.0f, 1.0f, 0.0f);
            glRotatef(stick.pitch, -1.0f, 0.0f, 0.0f);
            glTranslatef(0.0f, 0.0f, (stick.power * 3.0f) + 1.0f);
            glutSolidCylinder(0.2, 10.0f, 20, 20);
        glPopMatrix();
    glPopMatrix();

    // draw table
    glPushMatrix();
        glTranslatef(tableTopPos.x, tableTopPos.y, tableTopPos.z);
        glColor3f(0.1f, 0.3f, 0.1f);
        glScalef(tableTopHalfExtents.x * 2.f, tableTopHalfExtents.y * 2.f, tableTopHalfExtents.z * 2.f);
        glutSolidCube(1);
    glPopMatrix();
}

void draw_mini_map(void)
{
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);

    // set the projection matrix
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(
        -25, 25,
        -15, 15,
        0, 10
    );
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    gluLookAt(
        0, 10, 0,
        0, 0, 0,
        -1, 0, 0
    );
    render_scene();
    glPopMatrix();

    // restore the projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glPopAttrib();
}

void draw_power_bar(float percentage)
{
    // disable depth test for this overlay
    glPushAttrib(GL_DEPTH_TEST | GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    // window dimensions
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    // set the projection matrix
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // calculate width of the power bar
    float power_bar_max_width = (float) width / 4.0f;
    float power_bar_height = (float) height / 20.0f;

    // draw outline
    glColor3f(1.0f, 1.0f, 1.0f);
    glRectf(0.0f, 0.0f, power_bar_max_width, power_bar_height);

    // draw power bar
    glColor3f(1.0f, 0.0f, 0.0f);
    glRectf(0.0f, 0.0f, power_bar_max_width * percentage, power_bar_height);

    // render text
    static const char* text = "Power";
    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2f(power_bar_max_width + 10.0f, power_bar_height / 2.0f);
    for (int i = 0; text[i] != '\0'; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // restore depth test
    glPopAttrib();
}

void setup_lighting(void)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    // Set light properties
    GLfloat light_ambient[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat light_diffuse[]  = { 1.0f, 0.2f, 0.2f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // Set spotlight parameters
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0f); // Spotlight cone angle
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 4.5f); // Spotlight focus
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.8f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.001f);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // update camera
    update_view_matrix(&camera);

    // set up lighting
    GLfloat light_position[] = { 0.0f, 30.0f, 0.0f, 1.0f };
    GLfloat spot_direction[] = { 0.0f, -1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    setup_lighting();

    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    glViewport(0, 0, width, height);
    render_scene();

    glClear(GL_DEPTH_BUFFER_BIT);
    draw_power_bar(stick.power);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(2 * width / 3, 2 * height / 3, width / 3, height / 3); // set the viewport to the bottom left corner
    draw_mini_map();

    glutSwapBuffers();
}

void set_projection_matrix(int width, int height)
{
	GLdouble fov     = 38.0;    // degrees
	GLdouble aspect  = 1.0 * ((GLdouble) width / (GLdouble) height);     // aspect ratio aspect = height/width
	GLdouble nearVal = 0.5;
	GLdouble farVal  = 500.0;
	gluPerspective(fov, aspect, nearVal, farVal);
}

void resize_window(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// avoid division by zero
	h = (h == 0) ? 1 : h;
	w = (w == 0) ? 1 : w;

	// reset the camera
	set_projection_matrix(w, h);
	glMatrixMode(GL_MODELVIEW);
}


void opengl_init(void)
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // setup projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    set_projection_matrix(
        glutGet(GLUT_WINDOW_WIDTH),
        glutGet(GLUT_WINDOW_HEIGHT)
    );
    glMatrixMode(GL_MODELVIEW);
}

void app_init(void)
{
    // time
    time.update_rate = 120; // Hz
    time.last_frame_time = 0;
    time.current_frame_time = 0;

    // camera
    camera.radius = 100.0f;
    camera.pitch_angle = 30.0f;
    camera.yaw_angle = 90.0f;
    camera.target = (ac_vec3){0, 0, 0};

    // physics
    phys_init_world(&physicsWorld);
    physicsWorld.timeStep = 1.0f / time.update_rate;
    cueBallID = phys_add_entity(&physicsWorld, &default_cue_position);
    phys_make_entity_dynamic(&physicsWorld, cueBallID);

    tableTopID = phys_add_entity(&physicsWorld, &tableTopPos);
    phys_make_entity_static(&physicsWorld, tableTopID);

    ballCollider = (Collider){.type = SPHERE_C, .data = &ballRadius};

    tableTopCollider.type = AABB_C;
    tableTopCollider.data = &tableTopHalfExtents;

    phys_add_entity_collider(&physicsWorld, tableTopCollider, tableTopID);

    phys_add_entity_collider(&physicsWorld, ballCollider, cueBallID);

    // Set up pool ball formation
    int numRows = 4;
    float spacing = 2.f;
    float startZ = 0.0f;
    int ballIndex = 0;

    for (int row = 0; row < numRows; row++) {
        for (int col = 0; col <= row; col++) {
            ac_vec3 pos = (ac_vec3){
                .x = (col - row / 2.0f) * spacing,
                .y = 2.f,
                .z = startZ - row * spacing
            };

            ballIDs[ballIndex] = phys_add_entity(&physicsWorld, &pos);
            phys_add_entity_collider(&physicsWorld, ballCollider, ballIDs[ballIndex]);
            phys_make_entity_dynamic(&physicsWorld, ballIDs[ballIndex]);
            phys_add_collision_callback(&physicsWorld, ballIDs[ballIndex], collisionCallback);
            physicsWorld.masses[ballIndex] = 0.165f;
            ballIndex++;
        }
    }

    // stick
    stick.pitch = 0.0f;
    stick.yaw = 0.0f;
    stick.power = 0.5f;
    stick.target = cueBallID;
    stick.strike = false;
}

void app_update(int value)
{
    (void) value; // nullify unused value
    static const float msec_to_sec = 1.0f / 1000.0f;

    // get delta time
    time.current_frame_time = glutGet(GLUT_ELAPSED_TIME);
    float delta_time = (time.current_frame_time - time.last_frame_time) * msec_to_sec;
    time.last_frame_time = time.current_frame_time;

    // re-register timer callback
    glutTimerFunc(1000 / time.update_rate, app_update, 0);

    // update simulation
    if (stick.strike)
    {
        // apply force to target ball
        // force is between 0.0 and 1.0
        // max strike force is set to 60N (newtons)
        // and assuming instant impulse transfer
        // delta_v = F / m

        float force_newtons = 60.0f * stick.power; // 60N max force
        float mass_kg = physicsWorld.masses[stick.target];
        if (mass_kg > 0.0f)
        {
            float inv_mass = 1.0f / mass_kg;
            float delta_velocity = force_newtons * inv_mass;
            // float delta_v = 40.0f * stick.power; // max velocity is 5.0f

            // calculate direction of force based on stick orientation, yaw, and pitch
            ac_vec3 new_velocity = (ac_vec3){
                .x = -sinf(ac_deg_to_rad(stick.yaw)) * cosf(ac_deg_to_rad(stick.pitch)),
                .y = sinf(ac_deg_to_rad(stick.pitch)),
                .z = -cosf(ac_deg_to_rad(stick.yaw)) * cosf(ac_deg_to_rad(stick.pitch))
            };
            new_velocity = ac_vec3_normalize(&new_velocity);
            new_velocity = ac_vec3_scale(&new_velocity, delta_velocity);

            // apply to cue ball
            physicsWorld.velocities[stick.target] = new_velocity;
        }

        // reset stick power
        stick.power = 0.0f;
        stick.strike = false;
    }

    phys_update(&physicsWorld, delta_time);

    glutPostRedisplay();
}

void collisionCallback(unsigned e1, unsigned e2)
{
  printf("entity %u collided with entity %u\n", e1, e2);
}

void key_func(unsigned char key, int x, int y)
{
    (void) x; // nullify unused x
    (void) y; // nullify unused y

    static const float movement_step = 0.5f;
    static const float rotation_step = 2.0f;
    switch (key)
    {
    case '=':
        camera.radius = ac_clamp(camera.radius - movement_step, 10.0f, 100.0f);
        break;
    case '-':
        camera.radius = ac_clamp(camera.radius + movement_step, 10.0f, 100.0f);
        break;
    case 's':
    case 'S':
        stick.power = ac_clamp(stick.power + 0.01f, 0.0f, 1.0f);
        break;
    case 'w':
    case 'W':
        stick.power = ac_clamp(stick.power - 0.01f, 0.0f, 1.0f);
        break;
    case 'a':
    case 'A':
        stick.yaw += rotation_step;
        if (stick.yaw >= 360.0f)
        {
            stick.yaw -= 360.0f;
        }
        break;
    case 'd':
    case 'D':
        stick.yaw -= rotation_step;
        if (stick.yaw < 0.0f)
        {
            stick.yaw += 360.0f;
        }
        break;
    case 'q':
    case 'Q':
        stick.pitch = ac_clamp(stick.pitch + rotation_step, 0.0f, 90.0f);
        break;
    case 'e':
    case 'E':
        stick.pitch = ac_clamp(stick.pitch - rotation_step, 0.0f, 90.0f);
        break;
    case ' ':
        stick.strike = true;
        break;
    }
}

void special_key_func(int key, int x, int y)
{
    (void) x; // nullify unused x
    (void) y; // nullify unused y

    static const float rotation_step = 2.0f;

    switch (key)
    {
    case GLUT_KEY_UP:
        camera.pitch_angle = ac_clamp(camera.pitch_angle + rotation_step, 10.0f, 60.0f);
        break;
    case GLUT_KEY_DOWN:
        camera.pitch_angle = ac_clamp(camera.pitch_angle - rotation_step, 10.0f, 60.0f);
        break;
    case GLUT_KEY_LEFT:
        camera.yaw_angle += rotation_step;
        if (camera.yaw_angle >= 0.0f)
        {
            camera.yaw_angle -= 360.0f;
        }
        break;
    case GLUT_KEY_RIGHT:
        camera.yaw_angle -= rotation_step;
        if (camera.yaw_angle < 360.0f)
        {
            camera.yaw_angle += 360.0f;
        }
        break;
    default:
        break;
    }
}

int main(int argc, char **argv)
{
    // GLUT Initialisation
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(400, 100);
    glutCreateWindow("ace demo");
    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);

    // OpenGL Callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(resize_window);
    glutKeyboardFunc(key_func);
    glutSpecialFunc(special_key_func);

    opengl_init();
    app_init();

    glutTimerFunc( 1000 / time.update_rate, app_update, 0);
    time.last_frame_time = glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop();
}
