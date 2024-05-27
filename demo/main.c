
#include "GL/freeglut.h"

#include <stdio.h>

#include <ace/math/vec3.h>
#include <ace/physics/phys_world.h>
#include <ace/geometry/shapes.h>
#include <ace/render/camera.h>

#include "src/time.h"

PhysWorld physicsWorld;

Time time;

Camera* camera;

// entities

// balls (haha!)

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

void init(void);
void display(void);

void resize_window(int w, int h);

void collisionCallback(unsigned e1, unsigned e2);

//drawing (will probably need to be moved to src)

// void draw_balls();
// void draw_table();

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(400, 100);
    glutCreateWindow("ace demo");
    glutDisplayFunc(display);
    glutIdleFunc(display);

    glutReshapeFunc(resize_window);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);

    glEnable(GL_DEPTH_TEST);

    init();

    glutMainLoop(); /* enter event loop */
    // free colliders
}

void init(void)
{

    camera = camera_create();
    camera_init(camera);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

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

void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    update_time(&time, glutGet(GLUT_ELAPSED_TIME) / 1000.0f);

    //update camera..

    phys_update(&physicsWorld, time.deltaTime);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 30, 30, // needs to be camera
              0, 0, 0,
              0, 1, 0);

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

void resize_window(int w, int h) {
  // prevent a divide by zero
  if (h == 0)
    h = 1;
  float ratio = w * 1.0f / h;

  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();

  // set viewport to be the entire window
  glViewport(0, 0, w, h);

  // set perspective.
  gluPerspective(60.0f, ratio, 0.1f, 100.0f);

  glMatrixMode(GL_MODELVIEW);
}

void collisionCallback(unsigned e1, unsigned e2) {
  printf("entity %u collided with entity %u\n", e1, e2);
}