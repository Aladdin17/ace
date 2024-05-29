#include "render.h"
#include "types.h"
#include <ace/math/vec3.h>
#include <GL/freeglut.h>

void draw_pool_ball( const pool_ball* ball, const ac_vec3* position )
{
    // draw ball
    glPushMatrix();
    glTranslatef(position->x, position->y, position->z);
    glColor3f(ball->color.x, ball->color.y, ball->color.z);
    glutSolidSphere(ball->radius, 20, 20);
    glPopMatrix();
}

void draw_cue_stick( const cue_stick* stick, const ac_vec3* position, float radius )
{
    // draw stick
    glPushMatrix();
    glColor3f(0.651f, 0.51f, 0.035f); // brown
    glRotatef(stick->yaw, 0.0f, 1.0f, 0.0f);
    glRotatef(stick->pitch, 1.0f, 0.0f, 0.0f);
    glTranslatef(position->x, position->y, position->z); // move to cue ball
    glTranslatef(0.0f, 0.0f, (stick->power * 3.0f) + radius); // move to end of cue ball
    glutSolidCylinder(0.01, 1.45f, 20, 20); // 1cm, by 1.45m
    glPopMatrix();
}

void draw_scene( const pool_app* app )
{
    // app->table.draw(&app->table);

    for (int i = 0; i < app->num_balls; i++)
    {
        const ac_vec3* pos = &app->physics_world.positions[app->balls[i].physics_id];
        const pool_ball* ball = &app->balls[i];
        app->balls[i].draw(ball, pos);
    }

    if (app->cue_stick.visible)
    {
        const ac_vec3* target_pos = &app->physics_world.positions[app->cue_stick.target];
        const float target_radius = app->balls[app->cue_stick.target].radius;
        app->cue_stick.draw(&app->cue_stick, target_pos, target_radius);
    }
}

void draw_minimap(const pool_app* app)
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
    draw_scene(app);
    glPopMatrix();

    // restore the projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glPopAttrib();
}

void draw_powerbar(float power_ratio )
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
    glRectf(0.0f, 0.0f, power_bar_max_width * power_ratio, power_bar_height);

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
