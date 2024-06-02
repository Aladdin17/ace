/**
 * \file
 * \author Christien Alden
 * \author Blake Caldwell
 * \brief Implements the rendering functions for the application
 */
#include "render.h"
#include "types.h"
#include <GL/freeglut.h>
#include <ace/math/math.h>
#include <ace/math/vec3.h>
#include <stdio.h>

void draw_pool_ball(const pool_ball* ball, const ac_vec3* position)
{
    glPushMatrix();
    glTranslatef(position->x, position->y, position->z);
    glColor3f(ball->color.x, ball->color.y, ball->color.z);
    glutSolidSphere(ball->radius, 20, 20);
    glPopMatrix();
}

void draw_cue_stick(const cue_stick* stick, const ac_vec3* target_position, float target_radius)
{
    glPushMatrix();
    glColor3fv(stick->color.data);
    glTranslatef(target_position->x, target_position->y, target_position->z);
    glRotatef(stick->yaw, 0.0f, 1.0f, 0.0f);
    glRotatef(stick->pitch_angle, -1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.0f, stick->power * stick->draw_distance + target_radius);
    glutSolidCylinder(stick->radius, stick->length, 20, 20);
    glPopMatrix();
}

void draw_scene(const pool_app* app, bool orthographic)
{
    const PhysWorld* world     = &app->physics_world;
    const pool_ball* balls     = app->balls;
    const int        num_balls = app->num_balls;

    for ( int i = 0; i < app->num_balls; i++ )
    {
        // skip sleeping balls
        if ( app->physics_world.sleeping[app->balls[i].physics_id] )
        {
            continue;
        }
        const ac_vec3*   pos  = &app->physics_world.positions[app->balls[i].physics_id];
        const pool_ball* ball = &app->balls[i];
        app->balls[i].draw(ball, pos);
    }

    // draw the cue stick
    if ( app->cue_stick.visible )
    {
        const cue_stick* stick = &app->cue_stick;
        if ( stick->target_ball < num_balls )
        {
            const pool_ball* target_ball       = &balls[stick->target_ball];
            const unsigned   target_physics_id = target_ball->physics_id;
            const ac_vec3*   target_pos        = &world->positions[target_physics_id];
            const float      target_radius     = target_ball->radius;
            stick->draw(&app->cue_stick, target_pos, target_radius);
        }
    }

    app->table.draw(&app->table, orthographic);
}

void draw_minimap(const pool_app* app)
{
    glPushAttrib(GL_LIGHTING);
    glDisable(GL_LIGHTING);
    glClear(GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(-2, 2, -1, 1, 0, 3);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    gluLookAt(0, 2, 0, 0, 0, 0, -1, 0, 0);
    draw_scene(app, true);
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
}

void draw_powerbar(float power_ratio)
{
    // clamp the power ratio between the valid range, this should never be necessary
    power_ratio = ac_clamp(power_ratio, 0.0f, 1.0f);

    // disable depth test for this overlay
    glPushAttrib(GL_DEPTH_TEST | GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    // window dimensions
    int width  = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, width, 0.0, height);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // calculate width of the power bar
    float power_bar_max_width = (float) width / 4.0f;
    float power_bar_height    = (float) height / 20.0f;

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
    for ( int i = 0; text[i] != '\0'; i++ )
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // restore depth test and lighting
    glPopAttrib();
}

void draw_entity_info(const PhysWorld* world, unsigned target_ball_id)
{
    // push the current attributes
    glPushAttrib(GL_DEPTH_TEST | GL_LIGHTING);
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

    if ( target_ball_id < world->numEnts )
    {
        static char entity_buffer[256];

#pragma warning(push)
#pragma warning(disable : 4996)
        sprintf(
            entity_buffer,
            "Position (%.2f, %.2f, %.2f)\nVelocity (%.2f, %.2f, %.2f) | Speed: "
            "%2.3fm/s\nMass %.2fkg",
            world->positions[target_ball_id].x,
            world->positions[target_ball_id].y,
            world->positions[target_ball_id].z,
            world->velocities[target_ball_id].x,
            world->velocities[target_ball_id].y,
            world->velocities[target_ball_id].z,
            ac_vec3_magnitude(&world->velocities[target_ball_id]),
            world->masses[target_ball_id]
        );
#pragma warning(pop)

        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2d(10, glutGet(GLUT_WINDOW_HEIGHT) - 30);

        int line_height  = 20;
        int current_line = 0;

        for ( int i = 0; entity_buffer[i] != '\0'; i++ )
        {
            if ( entity_buffer[i] == '\n' )
            {
                current_line++;
                glRasterPos2d(10, glutGet(GLUT_WINDOW_HEIGHT) - 28 - current_line * line_height);
            }
            else
            {
                glutBitmapCharacter(GLUT_BITMAP_8_BY_13, entity_buffer[i]);
            }
        }
    }

    // restore the projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // restore the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glPopAttrib();
}

void draw_pool_table(const pool_table* table, bool orthographic)
{
    // draw table surface
    glPushMatrix();
    glColor3fv(table->surface_color.data);
    glTranslatef(0.0f, -table->top_depth / 2.0f, 0.0f);
    glScalef(table->width, table->top_depth, table->length);
    glutSolidCube(1.0f);
    glPopMatrix();

    // draw long cushions
    for ( int i = 0; i <= 1; i++ )
    {
        glPushMatrix();
        glColor3fv(table->cushion_color.data);
        glTranslatef(
            table->cushion_centers[i].x,
            table->cushion_centers[i].y,
            table->cushion_centers[i].z
        );
        glScalef(
            table->cushion_width,
            table->cushion_height,
            table->length + (2 * table->cushion_width)
        );
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    // draw short cushions
    for ( int i = 2; i <= 3; ++i )
    {
        glPushMatrix();
        glColor3fv(table->cushion_color.data);
        glTranslatef(
            table->cushion_centers[i].x,
            table->cushion_centers[i].y,
            table->cushion_centers[i].z
        );
        glScalef(
            table->width + (2 * table->cushion_width),
            table->cushion_height,
            table->cushion_width
        );
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    // draw pockets
    GLUquadric* quad = gluNewQuadric();
    for ( int i = 0; i < 4; ++i )
    {
        glPushMatrix();
        glColor3f(0, 0, 0);
        glTranslatef(
            table->pocket_centers[i].x,
            table->pocket_centers[i].y,
            table->pocket_centers[i].z
        );
        glScalef(1.2f, 0, 1.2f);
        gluSphere(quad, table->pocket_radius, 20, 20);
        glPopMatrix();
    }
    gluDeleteQuadric(quad);

    // draw legs
    if ( !orthographic )
    {
        for ( int i = 0; i < 4; ++i )
        {
            glPushMatrix();
            glColor3fv(table->leg_color.data);
            glTranslatef(table->leg_centers[i].x, table->leg_centers[i].y, table->leg_centers[i].z);
            glScalef(table->cushion_width, table->leg_length, table->cushion_width);
            glutSolidCube(1.0f);
            glPopMatrix();
            glPopMatrix();
        }
    }
}

void draw_controls_overlay(void)
{
    // push the current attributes
    glPushAttrib(GL_DEPTH_TEST | GL_LIGHTING);
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

    static char *entity_buffer =
"     C : Show Controls\n"
"   W/S : Set Cue Power\n"
"   A/D : Aim Cue\n"
"   Q/E : Pitch Cue\n"
" Space : Shoot\n"
"Arrows : Move Camera\n"
"   =/- : Zoom Camera\n"
"     M : Toggle Minimap\n"
"     I : Toggle Entity Info\n"
"   DEL : Reset\n"
"   ESC : Quit";

    const int x_offset    = glutGet(GLUT_WINDOW_WIDTH) - 230;
    const int y_offset    = 220;
    const int line_height = 20;

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2d(x_offset, y_offset);

    int   current_line = 0;
    char* next_char    = entity_buffer;
    while ( *next_char != '\0' )
    {
        if ( *next_char == '\n' )
        {
            current_line++;
            glRasterPos2d(x_offset, y_offset - (current_line * line_height));
        }
        else
        {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *next_char);
        }
        next_char++;
    }

    // restore the projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    // restore the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glPopAttrib();
}

void draw_init_screen(void)
{
    // clear the input buffer and screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // setup the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

    // setup the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // render text to center of screen
    static const char* text_line_1  = "Welcome to ACE Pool!";
    static const char* text_line_2  = "Please follow the instructions in the console";
    static const int   line_spacing = 20;

    // generate offsets
    const int y_offset_1 = glutGet(GLUT_WINDOW_HEIGHT) / 2 + line_spacing;
    const int text_length_1 =
        glutBitmapLength(GLUT_BITMAP_8_BY_13, (const unsigned char*) text_line_1) / 2;
    const int x_offset_1 = glutGet(GLUT_WINDOW_WIDTH) / 2 - text_length_1;

    const int y_offset_2 = glutGet(GLUT_WINDOW_HEIGHT) / 2 - line_spacing;
    const int text_length_2 =
        glutBitmapLength(GLUT_BITMAP_8_BY_13, (const unsigned char*) text_line_2) / 2;
    const int x_offset_2 = glutGet(GLUT_WINDOW_WIDTH) / 2 - text_length_2;

    // render text
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2d(x_offset_1, y_offset_1);
    for ( int i = 0; text_line_1[i] != '\0'; i++ )
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text_line_1[i]);
    }

    glRasterPos2d(x_offset_2, y_offset_2);
    for ( int i = 0; text_line_2[i] != '\0'; i++ )
    {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text_line_2[i]);
    }

    glutSwapBuffers();
}
