/**
 * \file
 * \author Christien Alden
 * \author Blake Caldwell
 * \brief Application main loop, initialisation, and callbacks.
 */
#include "app.h"
#include "init.h"
#include "render.h"
#include "types.h"
#include <GL/freeglut.h>
#include <ace/core/string.h>
#include <ace/geometry/shapes.h>
#include <ace/math/math.h>
#include <ace/math/vec3.h>
#include <ace/physics/phys_world.h>
#include <math.h>
#include <stdio.h>

//--------------------------------------------------------------------------------------------------
// Forward Declarations
//--------------------------------------------------------------------------------------------------

// user initialisation functions
int   get_num_balls_from_terminal(int);
int   get_layout_from_terminal(void);
float get_surface_roughness_from_terminal(void);
void  get_config_from_user(void);

// game logic
void ball_collision_callback(unsigned, unsigned);
void reset_target_ball_if_sleeping(void);
void update_cue_stick_visibility(void);
void detect_balls_off_table(void);
void strike_target_ball(void);
void update_main_camera(const orbit_camera*);
void setup_lighting(void);

// init functions
void app_cleanup(void);
void app_reset(void);

//--------------------------------------------------------------------------------------------------
// The app object and initialisation
//
// Not the best way to do this but we can't do much with the way GLUT requires the app to be global
//--------------------------------------------------------------------------------------------------

pool_app* app;

int get_num_balls_from_terminal(int max_balls)
{
    char input[256];
    int  num_balls = 0;
    bool valid     = false;
    while ( !valid )
    {
        printf("Enter the number of balls [0 - %d]: ", max_balls);
        if ( fgets(input, sizeof(input), stdin) != NULL )
        {
            removeNewlineChar(input);
            if ( strToUint32(input, (uint32_t*) &num_balls, 10) )
            {
                if ( num_balls >= 0 && num_balls <= max_balls )
                {
                    valid = true;
                }
            }
        }
    }

    return num_balls + 1;
}

int get_layout_from_terminal(void)
{
    char input[256];
    int  layout = 0;
    bool valid  = false;
    while ( !valid )
    {
        printf("Select the ball layout, 0 = Triangle, 1 = Rectangle: ");
        if ( fgets(input, sizeof(input), stdin) != NULL )
        {
            removeNewlineChar(input);
            if ( strToUint32(input, (uint32_t*) &layout, 10) )
            {
                switch ( layout )
                {
                case ball_layout_triangle:
                case ball_layout_rectangle:
                    valid = true;
                    break;
                }
            }
        }
    }

    return layout;
}

float get_surface_roughness_from_terminal(void)
{
    char  input[256];
    float roughness = 0.0f;
    bool  valid     = false;
    while ( !valid )
    {
        printf("Enter the surface roughness [0.0 - 1.0]: ");
        if ( fgets(input, sizeof(input), stdin) != NULL )
        {
            removeNewlineChar(input);
            if ( strToFloat(input, &roughness) )
            {
                if ( roughness >= 0.0f && roughness <= 1.0f )
                {
                    valid = true;
                }
            }
        }
    }

    return roughness;
}

void get_config_from_user(void)
{
    static const int max_balls = 55;  // fits well into the table...

    fflush(stdin);  // technically undefined behaviour but it works on most systems
    printf("Please insert numbers only, pressing enter after each input.\n\n");
    app->num_balls         = get_num_balls_from_terminal(max_balls);
    app->ball_layout       = get_layout_from_terminal();
    app->surface_roughness = get_surface_roughness_from_terminal();
    printf("Starting pool with %d balls\n\n\n", app->num_balls - 1);
}

frame_time* app_init(void)
{
    // allocate memory for the app and base initialisation
    app = malloc(sizeof(pool_app));
    draw_init_screen();
    get_config_from_user();

    // external initialisations
    initialise_misc(app);
    initialise_frame_time(&app->timer);
    initialise_orbit_camera(&app->main_camera);
    initialise_physics_world(&app->physics_world, app->timer.update_rate);

    // table must be initialised before balls
    initialise_pool_table(&app->physics_world, &app->table);
    initialise_pool_balls(
        &app->physics_world,
        &app->balls,
        app->num_balls,
        app->ball_layout,
        ball_collision_callback,
        &(ac_vec2){ app->table.width, app->table.length },
        &app->table.surface_center,
        &app->cue_start_position,
        &app->target_start_position,
        app->ball_drop_height
    );
    initialise_cue_stick(&app->cue_stick);

    // the caller needs this to register the timer callback
    return &app->timer;
}

void ball_collision_callback(unsigned body1, unsigned body2)
{
    unsigned* pockets     = app->table.pocket_physics_ids;
    int       num_pockets = 4;
    for ( int i = 0; i < num_pockets; ++i )
    {
        if ( body2 == pockets[i] )
        {
            // sleep the bodies
            app->physics_world.sleeping[body1]   = true;
            app->physics_world.velocities[body1] = ac_vec3_zero();
            app->physics_world.positions[body1]  = ac_vec3_zero();
        }
    }

    // contact with the surface
    if ( body2 == app->table.physics_ids[0] )
    {
        // we remove at most 2% of the velocity per frame
        const static float max_deprecation = 0.02f;
        float              scalar          = app->surface_roughness * max_deprecation;

        // only account for the x and z components of the velocity
        // we don't want to slow down the ball in the y direction
        // nor do we want to slow down the ball if it's already slow
        ac_vec3 velocity     = app->physics_world.velocities[body1];
        ac_vec2 velocity_xz  = { velocity.x, velocity.z };
        float   magnitude_xz = ac_vec2_magnitude(&velocity_xz);
        if ( magnitude_xz <= app->min_ball_speed )
        {
            return;
        }

        // apply the scalar to the velocity
        // 1.0f - scalar is the percentage of the velocity that will be kept
        // and apply this to the x and z components of the velocity
        float   new_velocity_magnitude = magnitude_xz * (1.0f - scalar);
        ac_vec2 normalised_velocity    = ac_vec2_normalize(&velocity_xz);
        ac_vec2 new_velocity_xz = ac_vec2_scale(&normalised_velocity, new_velocity_magnitude);
        ac_vec3 new_velocity =
            (ac_vec3){ .x = new_velocity_xz.x, .y = velocity.y, .z = new_velocity_xz.y };
        app->physics_world.velocities[body1] = new_velocity;
    }
}

void app_cleanup(void)
{
    if ( app )
    {
        if ( app->balls )
        {
            free(app->balls);
        }
        free(app);
    }
}

void app_reset(void)
{
    bool show_minimap = false;
    if ( app )
    {
        show_minimap = app->show_minimap;
    }
    app_cleanup();
    app_init();
    app->show_minimap = show_minimap;
}

//--------------------------------------------------------------------------------------------------
// Update Callback
//--------------------------------------------------------------------------------------------------

void app_update_callback(int value)
{
    (void) value;  // nullify unused value
    static const float msec_to_sec = 1.0f / 1000.0f;

    // get delta time and reregister callback
    app->timer.current_frame_time = glutGet(GLUT_ELAPSED_TIME);
    float delta_time = (app->timer.current_frame_time - app->timer.last_frame_time) * msec_to_sec;
    app->timer.last_frame_time = app->timer.current_frame_time;
    glutTimerFunc(1000 / app->timer.update_rate, app_update_callback, 0);

    // update simulation
    reset_target_ball_if_sleeping();
    update_cue_stick_visibility();
    detect_balls_off_table();
    strike_target_ball();
    phys_update(&app->physics_world, delta_time);

    glutPostRedisplay();
}

void reset_target_ball_if_sleeping(void)
{
    unsigned target_physics_id = app->balls[app->cue_stick.target_ball].physics_id;
    if ( app->physics_world.sleeping[target_physics_id] )
    {
        app->physics_world.sleeping[target_physics_id]   = false;
        // we apply a small downward velocity to help the stick not become
        // visible when the ball is reset
        app->physics_world.velocities[target_physics_id] = (ac_vec3){ 0.0f, -0.01f, 0.0f };
        app->physics_world.positions[target_physics_id]  = ball_start_pos_to_world_pos(
            &app->cue_start_position,
            &app->table.surface_center,
            &(ac_vec2){ app->table.width, app->table.length },
            app->ball_drop_height
        );
    }
}

void update_cue_stick_visibility(void)
{
    bool moving = false;
    for ( int i = 0; i < app->num_balls; i++ )
    {
        if ( app->physics_world.sleeping[app->balls[i].physics_id] )
        {
            continue;
        }

        if ( ac_vec3_magnitude(&app->physics_world.velocities[app->balls[i].physics_id]) >=
             app->min_ball_speed )
        {
            moving = true;
            break;
        }
    }
    app->cue_stick.visible = !moving;
}

void detect_balls_off_table(void)
{
    int target_ball_id = app->cue_stick.target_ball;
    for ( int i = 0; i < app->num_balls; i++ )
    {
        if ( app->physics_world.sleeping[app->balls[i].physics_id] )
        {
            continue;
        }

        ac_vec3* pos = &app->physics_world.positions[app->balls[i].physics_id];
        if ( pos->y < app->y_threshold )
        {
            if ( i == target_ball_id )
            {
                // the rest will be handled by the reset_target_ball_if_sleeping function
                app->physics_world.sleeping[app->balls[i].physics_id] = true;
                continue;
            }

            *pos = ball_start_pos_to_world_pos(
                &app->target_start_position,
                &app->table.surface_center,
                &(ac_vec2){ app->table.width, app->table.length },
                app->ball_drop_height
            );
            app->physics_world.velocities[app->balls[i].physics_id] = ac_vec3_zero();
        }
    }
}

void strike_target_ball(void)
{
    cue_stick* cue_stick = &app->cue_stick;

    if ( !cue_stick->strike )
    {
        return;
    }

    int num_balls   = app->num_balls;
    int target_ball = (int) cue_stick->target_ball;
    if ( target_ball >= num_balls )
    {
        return;
    }

    pool_ball*         balls                  = app->balls;
    PhysWorld*         world                  = &app->physics_world;
    unsigned           target_ball_physics_id = balls[target_ball].physics_id;
    float              mass_kg                = world->masses[target_ball_physics_id];
    static const float contact_time_seconds   = 0.01f;  // 10ms
    if ( mass_kg > 0.0f )
    {
        // calculate normalised direction of force based on stick orientation, yaw, and pitch
        ac_vec3 direction = (ac_vec3){
            .x = -sinf(ac_deg_to_rad(cue_stick->yaw)) * cosf(ac_deg_to_rad(cue_stick->pitch_angle)),
            .y = sinf(ac_deg_to_rad(cue_stick->pitch_angle)),
            .z = -cosf(ac_deg_to_rad(cue_stick->yaw)) * cosf(ac_deg_to_rad(cue_stick->pitch_angle))
        };

        direction = ac_vec3_normalize(&direction);

        // calculate the impulse based on power and direction
        ac_vec3 impulse =
            ac_vec3_scale(&direction, cue_stick->power * cue_stick->max_power_newtons);

        // calculate the acceleration of the ball based on 'F = ma' -> 'a = F/m'
        ac_vec3 acceleration = ac_vec3_scale(&impulse, 1.0f / mass_kg);

        // we will use the formula 'v = u + at' to calculate the new velocity
        // where 'u' is the initial velocity of the ball
        // 'a' is the acceleration of the ball
        // and 't' is the contact time between the stick and the ball

        // first we calculate the change in velocity based on acceleration and contact time
        // 'dv = a * t'
        ac_vec3 delta_velocity = ac_vec3_scale(&acceleration, contact_time_seconds);

        // update the velocity of the ball -> 'v = u + dv'
        world->velocities[target_ball_physics_id] =
            ac_vec3_add(&world->velocities[target_ball_physics_id], &delta_velocity);
    }

    // reset stick power
    cue_stick->power  = 0.0f;
    cue_stick->strike = false;
}

//--------------------------------------------------------------------------------------------------
// Key Callbacks
//--------------------------------------------------------------------------------------------------

void app_key_callback(unsigned char key, int x, int y)
{
    (void) x;  // nullify unused x
    (void) y;  // nullify unused y

    switch ( key )
    {
    case '=':
        app->main_camera.radius = ac_clamp(
            app->main_camera.radius - app->main_camera.zoom_step,
            app->main_camera.min_radius,
            app->main_camera.max_radius
        );
        break;
    case '-':
        app->main_camera.radius = ac_clamp(
            app->main_camera.radius + app->main_camera.zoom_step,
            app->main_camera.min_radius,
            app->main_camera.max_radius
        );
        break;
    case 's':
    case 'S':
        app->cue_stick.power =
            ac_clamp(app->cue_stick.power + app->cue_stick.power_step, 0.0f, 1.0f);
        break;
    case 'w':
    case 'W':
        app->cue_stick.power =
            ac_clamp(app->cue_stick.power - app->cue_stick.power_step, 0.0f, 1.0f);
        break;
    case 'a':
    case 'A':
        app->cue_stick.yaw += app->cue_stick.rotation_step;
        if ( app->cue_stick.yaw >= 360.0f )
        {
            app->cue_stick.yaw -= 360.0f;
        }
        break;
    case 'd':
    case 'D':
        app->cue_stick.yaw -= app->cue_stick.rotation_step;
        if ( app->cue_stick.yaw < 0.0f )
        {
            app->cue_stick.yaw += 360.0f;
        }
        break;
    case 'q':
    case 'Q':
        app->cue_stick.pitch_angle = ac_clamp(
            app->cue_stick.pitch_angle + app->cue_stick.rotation_step,
            app->cue_stick.min_pitch_angle,
            app->cue_stick.max_pitch_angle
        );
        break;
    case 'e':
    case 'E':
        app->cue_stick.pitch_angle = ac_clamp(
            app->cue_stick.pitch_angle - app->cue_stick.rotation_step,
            app->cue_stick.min_pitch_angle,
            app->cue_stick.max_pitch_angle
        );
        break;
    case ' ':
        if ( app->cue_stick.visible )
        {
            app->cue_stick.strike = true;
        }
        break;
    case 'c':
    case 'C':
        app->show_controls = !app->show_controls;
        break;
    case 'm':
    case 'M':
        app->show_minimap = !app->show_minimap;
        break;
    case 'i':
    case 'I':
        app->show_entity_info = !app->show_entity_info;
        break;
    case 27:  // ESCAPE Key
        glutLeaveMainLoop();
        break;
    case 127:  // DELETE Key
        app_reset();
        break;
    }
}

void app_special_key_callback(int key, int x, int y)
{
    (void) x;  // nullify unused x
    (void) y;  // nullify unused y

    switch ( key )
    {
    case GLUT_KEY_UP:
        app->main_camera.pitch_angle = ac_clamp(
            app->main_camera.pitch_angle + app->main_camera.rotation_step,
            app->main_camera.min_pitch_angle,
            app->main_camera.max_pitch_angle
        );
        break;
    case GLUT_KEY_DOWN:
        app->main_camera.pitch_angle = ac_clamp(
            app->main_camera.pitch_angle - app->main_camera.rotation_step,
            app->main_camera.min_pitch_angle,
            app->main_camera.max_pitch_angle
        );
        break;
    case GLUT_KEY_LEFT:
        app->main_camera.yaw_angle += app->main_camera.rotation_step;
        if ( app->main_camera.yaw_angle >= 0.0f )
        {
            app->main_camera.yaw_angle -= 360.0f;
        }
        break;
    case GLUT_KEY_RIGHT:
        app->main_camera.yaw_angle -= app->main_camera.rotation_step;
        if ( app->main_camera.yaw_angle < 360.0f )
        {
            app->main_camera.yaw_angle += 360.0f;
        }
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------------------------------------------
// High-Level Rendering
//--------------------------------------------------------------------------------------------------

void app_render_callback(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // update camera and lighting
    update_main_camera(&app->main_camera);
    setup_lighting();

    int width  = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    // draw the scene in the main viewport
    glViewport(0, 0, width, height);
    draw_scene(app, false);

    if ( app->show_entity_info )
    {
        unsigned target_entity = app->balls[0].physics_id;
        draw_entity_info(&app->physics_world, target_entity);
    }

    // draw the powerbar overlay
    draw_powerbar(app->cue_stick.power);

    // render the minimap overlay
    if ( app->show_minimap )
    {
        // set the viewport to the top right corner
        glViewport(2 * width / 3, 2 * height / 3, width / 3, height / 3);
        draw_minimap(app);
    }

    if ( app->show_controls )
    {
        glViewport(0, 0, width, height);
        draw_controls_overlay();
    }

    glutSwapBuffers();
}

void update_main_camera(const orbit_camera* cam)
{
    float pitch_rad = ac_deg_to_rad(cam->pitch_angle);
    float yaw_rad   = ac_deg_to_rad(cam->yaw_angle);

    ac_vec3 up      = (ac_vec3){ 0, 1, 0 };
    ac_vec3 radial  = (ac_vec3){ .x = cosf(yaw_rad) * cosf(pitch_rad),
                                 .y = sinf(pitch_rad),
                                 .z = sinf(yaw_rad) * cosf(pitch_rad) };
    radial          = ac_vec3_normalize(&radial);
    radial          = ac_vec3_scale(&radial, cam->radius);
    ac_vec3 eye     = ac_vec3_add(&cam->target, &radial);
    ac_vec3 look_at = cam->target;

    gluLookAt(eye.x, eye.y, eye.z, look_at.x, look_at.y, look_at.z, up.x, up.y, up.z);
}

void setup_lighting(void)
{
    // lighting properties
    static const GLfloat light1_position[]       = { 0.0f, 3.0f, 0.0f, 1.0f };
    static const GLfloat light1_ambient[]        = { 0.5f, 0.5f, 0.5f, 1.0f };
    static const GLfloat light1_diffuse[]        = { 0.3f, 0.3f, 0.3f, 1.0f };
    static const GLfloat light1_specular[]       = { 0.7f, 0.7f, 0.7f, 1.0f };
    static const GLfloat light1_spot_direction[] = { 0.0f, -1.0f, 0.0f };
    static const GLfloat light1_spot_cutoff      = 45.0f;
    static const GLfloat light1_spot_exponent    = 6.0f;

    // enable lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    glLightfv(GL_LIGHT0, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light1_spot_direction);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light1_specular);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, light1_spot_cutoff);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, light1_spot_exponent);
}
