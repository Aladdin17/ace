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
    glTranslatef(position->x, position->y, position->z); // move to cue ball
    glRotatef(stick->yaw, 0.0f, 1.0f, 0.0f);
    glRotatef(stick->pitch, -1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, (stick->power) / 8.0f + radius); // move to end of cue ball
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
        -2, 2,
        -1, 1,
        0, 2
    );
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    gluLookAt(
        0, 2, 0,
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

void draw_entity_info(const pool_app* app, unsigned entity_id)
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    
    // setup the projection matrix
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

    // setup the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // disable lighting and depth testing
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    PhysWorld* world = &app->physics_world;


    if (entity_id < app->physics_world.numEnts)
    {
        static char * bool_string[] = {"False", "True"};
        static char entity_buffer[256];
        sprintf(entity_buffer, "Entity %u | Tag: %s\n\tPosition (%.2f, %.2f, %.2f)\n\tVelocity (%.2f, %.2f, %.2f) | Speed: %2.fm/s\n\tMass %.2f\n\tSleeping: %s",
                entity_id,
                "place holder",
                world->positions[entity_id].x, world->positions[entity_id].y, world->positions[entity_id].z,
                world->velocities[entity_id].x, world->velocities[entity_id].y, world->velocities[entity_id].z, ac_vec3_magnitude(&world->velocities[entity_id]),
                world->masses[entity_id],
                bool_string[world->sleeping[entity_id]]);

        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2d(10, glutGet(GLUT_WINDOW_HEIGHT) - 30);

        int line_height = 20;
        int current_line = 0;

        for (int i = 0; entity_buffer[i] != '\0'; i++)
        {
            if (entity_buffer[i] == '\n')
            {
                current_line++;
                glRasterPos2d(10, glutGet(GLUT_WINDOW_HEIGHT) - 28 - current_line * line_height);
            }
            else
            {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, entity_buffer[i]);
            }
        }
    }


    // reenable lighting and depth testing
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    // restore the projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // restore the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
