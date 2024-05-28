
#include "GL/freeglut.h"

#include <stdio.h>

#include <ace/math/vec3.h>
#include <ace/physics/phys_world.h>
#include <ace/geometry/shapes.h>
#include <ace/render/camera.h>

typedef struct
{
    int update_rate;
    int last_frame_time;
    int current_frame_time;
} frame_time;

PhysWorld physicsWorld;
frame_time time;
ac_camera* camera;

// entities
unsigned cueBallID;
ac_vec3 cuePos = {0, 2, 15};
Collider ballCollider;

unsigned ballIDs[10];
ac_vec3 ballPositions[10];

float ballRadius = 1; // used for all the balls so their size is uniform

// table

unsigned tableTopID;
ac_vec3 tableTopPos = {0, 0, 0};
Collider tableTopCollider;
ac_vec3 tableTopHalfExtents = {20, 1, 20};

// functions
void collisionCallback(unsigned e1, unsigned e2);

void display(void)
{
    // update the camera
    ac_camera_update_orientation(camera);
    ac_camera_update_position(camera);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    // setup camera
    ac_vec3 camera_pos = ac_camera_get_position(camera);
    ac_vec3 camera_front = ac_camera_get_front(camera);
    ac_vec3 camera_up = ac_camera_get_up(camera);
    ac_vec3 look_at = ac_vec3_add(&camera_pos, &camera_front);
    gluLookAt(
        camera_pos.x, camera_pos.y, camera_pos.z,
        look_at.x, look_at.y, look_at.z,
        camera_up.x, camera_up.y, camera_up.z
    );

    // draw entities
    for (int i = 0; i < 10; i++)
    {
        glPushMatrix();
        glTranslatef(ballPositions[i].x, ballPositions[i].y, ballPositions[i].z);
        glColor3f(0.4f, 0.1f, 0.1f);
        glutSolidSphere(1.0, 20, 20);
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(cuePos.x, cuePos.y, cuePos.z);
    glColor3f(0.8f, 0.8f, 0.8f);

    glutSolidSphere(1, 20, 20);

    glPopMatrix();

    glPushMatrix();

    glTranslatef(tableTopPos.x, tableTopPos.y, tableTopPos.z);
    glColor3f(0.1f, 0.3f, 0.1f);

    glScalef(tableTopHalfExtents.x * 2.f, tableTopHalfExtents.y * 2.f, tableTopHalfExtents.z * 2.f);

    glutSolidCube(1);

    glPopMatrix();

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
    time.update_rate = 60; // Hz
    time.last_frame_time = 0;
    time.current_frame_time = 0;

    // camera
    camera = ac_camera_create();
    ac_camera_init(camera);

    // physics
    phys_init_world(&physicsWorld);
    cueBallID = phys_add_entity(&physicsWorld, &cuePos);
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
            ballPositions[ballIndex] = (ac_vec3){
                .x = (col - row / 2.0f) * spacing,
                .y = 2.f,
                .z = startZ - row * spacing
            };

            ballIDs[ballIndex] = phys_add_entity(&physicsWorld, &ballPositions[ballIndex]);
            phys_add_entity_collider(&physicsWorld, ballCollider, ballIDs[ballIndex]);
            phys_make_entity_dynamic(&physicsWorld, ballIDs[ballIndex]);
            phys_add_collision_callback(&physicsWorld, ballIDs[ballIndex], collisionCallback);
            ballIndex++;
        }
    }

    physicsWorld.velocities[cueBallID].z = -20.f;
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
    phys_update(&physicsWorld, delta_time);
    glutPostRedisplay();
}

void collisionCallback(unsigned e1, unsigned e2)
{
  printf("entity %u collided with entity %u\n", e1, e2);
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

    opengl_init();
    app_init();

    glutTimerFunc( 1000 / time.update_rate, app_update, 0);
    time.last_frame_time = glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop();
}
