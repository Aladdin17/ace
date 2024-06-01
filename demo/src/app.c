#include "app.h"
#include "init.h"
#include "render.h"
#include "types.h"
#include <ace/geometry/shapes.h>
#include <ace/math/math.h>
#include <ace/math/vec3.h>
#include <ace/physics/phys_world.h>
#include <GL/freeglut.h>
#include <math.h>

//--------------------------------------------------------------------------------------------------
// Forward Declarations
//--------------------------------------------------------------------------------------------------
void ball_collision_callback(unsigned, unsigned);
void update_cue_stick_visibility(void);
void detect_balls_off_table(void);
void strike_target_ball(void);
void update_main_camera(const orbit_camera*);
void setup_lighting(void);

//--------------------------------------------------------------------------------------------------
// The app object and initialisation
//
// Not the best way to do this but we can't do much with the way GLUT requires the app to be global
//--------------------------------------------------------------------------------------------------

pool_app* app;

frame_time* app_init( void )
{
    app = malloc(sizeof(pool_app));

    // frame_time
    app->timer.update_rate = 120; // Hz
    app->timer.last_frame_time = 0;
    app->timer.current_frame_time = 0;

    // show minimap
    app->show_minimap = false;

    //show info
    app->show_entity_info = false;
    app->target_entity_info = 1;

    // external initialisations
    initialise_orbit_camera(&app->main_camera);
    initialise_physics_world(&app->physics_world, app->timer.update_rate);

    app->num_balls = 10; //this includes the cue ball because of init
    app->balls = malloc(sizeof(pool_ball) * app->num_balls);
    initialise_pool_balls(&app->physics_world, app->balls, app->num_balls);
    for (int i = 0; i < app->num_balls; i++)
    {
        phys_add_collision_callback(&app->physics_world, app->balls[i].physics_id, ball_collision_callback);
    }
    initialise_pool_table(&app->physics_world, &app->table);
    initialise_cue_stick(&app->cue_stick);
    return &app->timer;
}

void ball_collision_callback(unsigned body1, unsigned body2)
{
    unsigned* pockets = app->table.pocket_physics_ids;
    int num_pockets = 4;
    for (int i = 0; i < num_pockets; ++i)
    {
        if (body2 == pockets[i])
        {
            if (body1 == 0) // cue-ball
            {
                app->physics_world.velocities[app->balls[i].physics_id] = (ac_vec3){0.0f, 0.0f, 0.0f};
                app->physics_world.positions[body1] = (ac_vec3){0.0f, 0.2f, 0.55f};
            }
            else
            {
                // sleep body 1 and move to origin
                app->physics_world.velocities[body1] = (ac_vec3){ 0.0f, -1.0f, 0.0f };
                app->physics_world.sleeping[body1] = true;
                app->physics_world.positions[body1] = ac_vec3_zero();
            }
        }
    }
}

void app_reset( void )
{
    bool show_minimap = false;
    if (app)
    {
        show_minimap = app->show_minimap;
        free(app);
    }

    app_init();
    app->show_minimap = show_minimap;
}

//--------------------------------------------------------------------------------------------------
// Update Callback
//--------------------------------------------------------------------------------------------------

void app_update_callback( int value )
{
    (void) value; // nullify unused value
    static const float msec_to_sec = 1.0f / 1000.0f;

    // get delta time and reregister callback
    app->timer.current_frame_time = glutGet(GLUT_ELAPSED_TIME);
    float delta_time = (app->timer.current_frame_time - app->timer.last_frame_time) * msec_to_sec;
    app->timer.last_frame_time = app->timer.current_frame_time;
    glutTimerFunc(1000 / app->timer.update_rate, app_update_callback, 0);

    // update simulation
    update_cue_stick_visibility();
    detect_balls_off_table();
    strike_target_ball();
    phys_update(&app->physics_world, delta_time);

    glutPostRedisplay();
}

void update_cue_stick_visibility(void)
{
    // show cue stick when cue ball not moving
    bool moving = false;
    for (int i = 0; i < app->num_balls; i++)
    {
        if (app->physics_world.sleeping[app->balls[i].physics_id])
        {
            continue;
        }

        if (ac_vec3_magnitude(&app->physics_world.velocities[app->balls[i].physics_id]) >= 0.07f)
        {
            moving = true;
            break;
        }
    }
    app->cue_stick.visible = !moving;
}

void detect_balls_off_table(void)
{
    // detect if a ball has dropped into a 'pocket'
    const float y_threshold = -2.0f;

    for (int i = 0; i < app->num_balls; i++)
    {
        ac_vec3* pos = &app->physics_world.positions[app->balls[i].physics_id];
        if (pos->y < y_threshold)
        {
            if (i == 0)
            {
                // cue ball dropped, reset to origin
                *pos = (ac_vec3){0.0f, 0.2f, 0.55f};
                app->physics_world.velocities[app->balls[i].physics_id] = (ac_vec3){0.0f, 0.0f, 0.0f};
            }
            else
            {
                // target ball dropped
                // sleep the ball and move to origin
                app->physics_world.velocities[app->balls[i].physics_id] = (ac_vec3){0.0f, 0.0f, 0.0f};
                app->physics_world.sleeping[app->balls[i].physics_id] = true;
                *pos = (ac_vec3){ 0.0f, -1.0f, 0.0f };
            }
        }
    }
}

void strike_target_ball(void)
{
    if (app->cue_stick.strike)
    {
        // apply force to target ball
        // force is between 0.0 and 1.0
        // max strike force is set to 60N (newtons)
        // and assuming instant impulse transfer
        // delta_v = F / m

        // float force_newtons = 60.0f * app->cue_stick.power; // 60N max force
        float mass_kg = app->physics_world.masses[app->cue_stick.target];
        if (mass_kg > 0.0f)
        {
            float max_velocity = 5.0f; // m/s

            // float inv_mass = 1.0f / mass_kg * 1000.0f;
            // float delta_velocity = force_newtons * inv_mass;
            // float delta_v = 40.0f * stick.power; // max velocity is 5.0f

            // calculate direction of force based on stick orientation, yaw, and pitch
            ac_vec3 new_velocity = (ac_vec3){
                .x = -sinf(ac_deg_to_rad(app->cue_stick.yaw)) * cosf(ac_deg_to_rad(app->cue_stick.pitch)),
                .y = sinf(ac_deg_to_rad(app->cue_stick.pitch)),
                .z = -cosf(ac_deg_to_rad(app->cue_stick.yaw)) * cosf(ac_deg_to_rad(app->cue_stick.pitch))
            };
            new_velocity = ac_vec3_normalize(&new_velocity);
            new_velocity = ac_vec3_scale(&new_velocity, max_velocity * app->cue_stick.power);

            // apply to cue ball
            app->physics_world.velocities[app->cue_stick.target] = new_velocity;
        }

        // reset stick power
        app->cue_stick.power = 0.0f;
        app->cue_stick.strike = false;
    }
}

//--------------------------------------------------------------------------------------------------
// Key Callbacks
//--------------------------------------------------------------------------------------------------

void app_key_callback(unsigned char key, int x, int y)
{
    (void) x; // nullify unused x
    (void) y; // nullify unused y

    static const float movement_step = 0.05f;
    static const float rotation_step = 1.0f;
    switch (key)
    {
    case '=':
        app->main_camera.radius = ac_clamp(app->main_camera.radius - movement_step, 1.0f, 10.0f);
        break;
    case '-':
        app->main_camera.radius = ac_clamp(app->main_camera.radius + movement_step, 1.0f, 10.0f);
        break;
    case 's':
    case 'S':
        app->cue_stick.power = ac_clamp(app->cue_stick.power + 0.01f, 0.0f, 1.0f);
        break;
    case 'w':
    case 'W':
        app->cue_stick.power = ac_clamp(app->cue_stick.power - 0.01f, 0.0f, 1.0f);
        break;
    case 'a':
    case 'A':
        app->cue_stick.yaw += rotation_step;
        if (app->cue_stick.yaw >= 360.0f)
        {
            app->cue_stick.yaw -= 360.0f;
        }
        break;
    case 'd':
    case 'D':
        app->cue_stick.yaw -= rotation_step;
        if (app->cue_stick.yaw < 0.0f)
        {
            app->cue_stick.yaw += 360.0f;
        }
        break;
    case 'q':
    case 'Q':
        app->cue_stick.pitch = ac_clamp(app->cue_stick.pitch + rotation_step, 0.0f, 90.0f);
        break;
    case 'e':
    case 'E':
        app->cue_stick.pitch = ac_clamp(app->cue_stick.pitch - rotation_step, 0.0f, 90.0f);
        break;
    case ' ':
        if (app->cue_stick.visible)
        {
            app->cue_stick.strike = true;
        }
        break;
    case 'm':
    case 'M':
        app->show_minimap = !app->show_minimap;
        break;
    case 27: // ESCAPE Key
        glutLeaveMainLoop();
        break;
    case 127: // DELETE Key
        app_reset();
        break;
    }
}

void app_special_key_callback(int key, int x, int y)
{
    (void) x; // nullify unused x
    (void) y; // nullify unused y

    static const float rotation_step = 1.0f;

    switch (key)
    {
    case GLUT_KEY_UP:
        app->main_camera.pitch_angle = ac_clamp(app->main_camera.pitch_angle + rotation_step, 10.0f, 60.0f);
        break;
    case GLUT_KEY_DOWN:
        app->main_camera.pitch_angle = ac_clamp(app->main_camera.pitch_angle - rotation_step, 10.0f, 60.0f);
        break;
    case GLUT_KEY_LEFT:
        app->main_camera.yaw_angle += rotation_step;
        if (app->main_camera.yaw_angle >= 0.0f)
        {
            app->main_camera.yaw_angle -= 360.0f;
        }
        break;
    case GLUT_KEY_RIGHT:
        app->main_camera.yaw_angle -= rotation_step;
        if (app->main_camera.yaw_angle < 360.0f)
        {
            app->main_camera.yaw_angle += 360.0f;
        }
        break;
    case GLUT_KEY_F12:
        app->show_entity_info = !app->show_entity_info;
        break;
    default:
        break;
    }


    if(key >= GLUT_KEY_F1 && key <= GLUT_KEY_F11)
    {
        app->show_entity_info;
        app->target_entity_info = key - 1;
        return;
    }

}

//--------------------------------------------------------------------------------------------------
// High-Level Rendering
//--------------------------------------------------------------------------------------------------

void app_render_callback( void )
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // update camera
    update_main_camera(&app->main_camera);

    // set up lighting
    GLfloat light_position[] = { 0.0f, 3.0f, 0.0f, 1.0f };
    GLfloat spot_direction[] = { 0.0f, -1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    setup_lighting();

    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    // draw the scene in the main viewport
    glViewport(0, 0, width, height);
    draw_scene(app, false);

    if(app->show_entity_info)
    {
        draw_entity_info(app, app->target_entity_info);
    }

    // draw the powerbar overlay
    draw_powerbar(app->cue_stick.power);

    // render the minimap overlay
    if (app->show_minimap)
    {
        // set the viewport to the top right corner
        glViewport(2 * width / 3, 2 * height / 3, width / 3, height / 3);
        draw_minimap(app);
    }

    glutSwapBuffers();
}

void update_main_camera(const orbit_camera* cam)
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

void setup_lighting(void)
{
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    // Set light properties
    GLfloat light_ambient[]  = { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat light_diffuse[]  = { 0.3f, 0.3f, 0.3f, 1.0f };
    GLfloat light_specular[] = { 0.7f, 0.7f, 0.7f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    // Set spotlight parameters
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0f); // Spotlight cone angle
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 6.0f); // Spotlight focus
}
