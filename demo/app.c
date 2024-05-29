#include "app.h"
#include <ace/geometry/shapes.h>
#include <ace/math/math.h>
#include <ace/math/vec3.h>
#include <ace/physics/phys_world.h>
#include <math.h>
#include <GL/freeglut.h>

typedef struct
{
    float radius;
    float pitch_angle;
    float yaw_angle;
    ac_vec3 target;
} orbit_camera;

typedef struct cue_stick
{
    unsigned target;
    float pitch;
    float yaw;
    float power;
    bool strike;
    bool visible;
    void (*draw)(struct cue_stick*);
} cue_stick;

typedef struct pool_ball
{
    unsigned physics_id;        // physics id for lookup of properties
    ac_vec3 color;              // color of the ball
    float radius;               // radius of the ball
    char tag[16];               // 15 char identitier
    void (*draw)(struct pool_ball*); // function pointer to draw the ball
} pool_ball;

typedef struct pool_table
{
    unsigned physics_ids[6];           // physics id for lookup of properties
    void (*draw)(struct pool_table*);  // function pointer to draw the table
} pool_table;

typedef struct pool_app
{
    frame_time timer;          // stores frame time information
    orbit_camera main_camera;  // used to orbit the camera around the table
    cue_stick stick;           // cue stick
    pool_ball balls[11];       // 0 is cue ball, plus 10 targets
    int num_balls;             // number of balls
    pool_table table;          // pool table
    PhysWorld physics_world;         // physics world
} pool_app;

//--------------------------------------------------------------------------------------------------
// The app object and initialisation
//
// Not the best way to do this but we can't do much with the way GLUT requires the app to be global
//--------------------------------------------------------------------------------------------------
pool_app* app;

typedef struct
{
    ac_vec3 pos;
    ac_vec3 color;
    char tag[16];
    float mass;
    float radius;
} ball_info;

static ball_info ball_setup[] = {
    { {0.0f, -0.91f, 0.80f}, {1.0f, 1.0f, 1.0f}, "Cue", 0.170f, 0.05715f},
    { {0.0f, -0.91f, 0.80f}, {1.0f, 1.0f, 1.0f}, "1", 0.170f, 0.05715f},
    { {0.0f, -0.91f, 0.80f}, {1.0f, 1.0f, 1.0f}, "2", 0.170f, 0.05715f},
    { {0.0f, -0.91f, 0.80f}, {1.0f, 1.0f, 1.0f}, "3", 0.170f, 0.05715f},
    { {0.0f, -0.91f, 0.80f}, {1.0f, 1.0f, 1.0f}, "4", 0.170f, 0.05715f},
    { {0.0f, -0.91f, 0.80f}, {1.0f, 1.0f, 1.0f}, "5", 0.170f, 0.05715f},
    { {0.0f, -0.91f, 0.80f}, {1.0f, 1.0f, 1.0f}, "6", 0.170f, 0.05715f},
    { {0.0f, -0.91f, 0.80f}, {1.0f, 1.0f, 1.0f}, "7", 0.170f, 0.05715f},
    { {0.0f, -0.91f, 0.80f}, {1.0f, 1.0f, 1.0f}, "8", 0.170f, 0.05715f},
    { {0.0f, -0.91f, 0.80f}, {1.0f, 1.0f, 1.0f}, "9", 0.170f, 0.05715f},
    { {0.0f, -0.91f, 0.80f}, {1.0f, 1.0f, 1.0f}, "10", 0.170f, 0.05715f}
};

void draw_cue_stick( cue_stick* stick )
{
    ac_vec3 position = app->physics_world.positions[stick->target];
    float target_radius = app->balls[stick->target].radius;

    // draw stick
    glPushMatrix();
    glColor3f(0.651f, 0.51f, 0.035f); // brown
    glRotatef(stick->yaw, 0.0f, 1.0f, 0.0f);
    glRotatef(stick->pitch, 1.0f, 0.0f, 0.0f);
    glTranslatef(position.x, position.y, position.z); // move to cue ball
    glTranslatef(0.0f, 0.0f, (stick->power * 3.0f) + target_radius); // move to end of cue ball
    glutSolidCylinder(0.01, 1.45f, 20, 20); // 1cm, by 1.45m
    glPopMatrix();
}

void initialise_physics_world( void )
{
    PhysWorld* world = &app->physics_world;
    phys_init_world(world);
    world->timeStep = 1.0f / app->timer.update_rate;
}

void draw_pool_ball( pool_ball* ball )
{
    ac_vec3 position = app->physics_world.positions[ball->physics_id];
    // draw ball
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glColor3f(ball->color.x, ball->color.y, ball->color.z);
    glutSolidSphere(ball->radius, 20, 20);
    glPopMatrix();
}

void initialise_pool_balls( void )
{
    PhysWorld* world = &app->physics_world;

    // currently all balls have the same radius but can differ in mass
    static Sphere sphere_collider = { .radius = 0.0305f  };
    static const Collider collider = { .type = SPHERE_C, .data = &sphere_collider };

    app->num_balls = 11;
    for (int i = 0; i < app->num_balls; ++i)
    {
        ball_info* info = &ball_setup[i];

        // add to physics world
        unsigned ballID = phys_add_entity(world, &info->pos);
        phys_add_entity_collider(world, collider, ballID);
        phys_make_entity_dynamic(world, ballID);
        // phys_add_collision_callback(world, ballID, collisionCallback);
        world->masses[ballID] = info->mass;

        // add to app structure
        app->balls[i].physics_id = ballID;
        app->balls[i].color = info->color;
        app->balls[i].radius = info->radius;
        strcpy(app->balls[i].tag, info->tag);
        app->balls[i].draw = draw_pool_ball; // draw_ball;
    }
}

void initialise_pool_table( PhysWorld* world )
{
    unsigned tableID = phys_add_entity(world, &(ac_vec3){ 0.0f, 0.0f, 0.0f});
    phys_make_entity_static(world, tableID);

//     tableTopCollider.type = AABB_C;
//     tableTopCollider.data = &tableTopHalfExtents;

//     phys_add_entity_collider(&physicsWorld, tableTopCollider, tableTopID);
}

void initialise_orbit_cam()
{
    app->main_camera.radius = 2.0f;
    app->main_camera.pitch_angle = 30.0f;
    app->main_camera.yaw_angle = 90.0f;
    app->main_camera.target = (ac_vec3){0, 0, 0};
}

void initialise_cue_stick( void )
{
    app->stick.pitch = 0.0f;
    app->stick.yaw = 0.0f;
    app->stick.power = 0.0f;
    app->stick.target = 0;  // cue ball
    app->stick.strike = false;
    app->stick.visible = true;
    app->stick.draw = draw_cue_stick;
}

void initialise_timer( void )
{
    app->timer.update_rate = 120; // Hz
    app->timer.last_frame_time = 0;
    app->timer.current_frame_time = 0;
}

frame_time* app_init( void )
{
    app = malloc(sizeof(pool_app));
    initialise_timer();
    initialise_orbit_cam();
    initialise_physics_world();
    initialise_pool_balls();
    initialise_cue_stick();
    return &app->timer;
}

//--------------------------------------------------------------------------------------------------
// App callbacks
//--------------------------------------------------------------------------------------------------

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

void render_scene()
{
    // draw balls
    for (int i = 0; i < app->num_balls; i++)
    {
        app->balls[i].draw(&app->balls[i]);
    }

    if (app->stick.visible)
    {
        app->stick.draw(&app->stick);
    }

    // draw table
    // glPushMatrix();
    //     glTranslatef(tableTopPos.x, tableTopPos.y, tableTopPos.z);
    //     glColor3f(0.1f, 0.3f, 0.1f);
    //     glScalef(tableTopHalfExtents.x * 2.f, tableTopHalfExtents.y * 2.f, tableTopHalfExtents.z * 2.f);
    //     glutSolidCube(1);
    // glPopMatrix();
}

void app_update( int value )
{
    (void) value; // nullify unused value
    static const float msec_to_sec = 1.0f / 1000.0f;

    // get delta time
    app->timer.current_frame_time = glutGet(GLUT_ELAPSED_TIME);
    float delta_time = (app->timer.current_frame_time - app->timer.last_frame_time) * msec_to_sec;
    app->timer.last_frame_time = app->timer.current_frame_time;

    // re-register timer callback
    glutTimerFunc(1000 / app->timer.update_rate, app_update, 0);

    // update simulation
    if (app->stick.strike)
    {
        // apply force to target ball
        // force is between 0.0 and 1.0
        // max strike force is set to 60N (newtons)
        // and assuming instant impulse transfer
        // delta_v = F / m

        float force_newtons = 60.0f * app->stick.power; // 60N max force
        float mass_kg = app->physics_world.masses[app->stick.target];
        if (mass_kg > 0.0f)
        {
            float inv_mass = 1.0f / mass_kg;
            float delta_velocity = force_newtons * inv_mass;
            // float delta_v = 40.0f * stick.power; // max velocity is 5.0f

            // calculate direction of force based on stick orientation, yaw, and pitch
            ac_vec3 new_velocity = (ac_vec3){
                .x = -sinf(ac_deg_to_rad(app->stick.yaw)) * cosf(ac_deg_to_rad(app->stick.pitch)),
                .y = sinf(ac_deg_to_rad(app->stick.pitch)),
                .z = -cosf(ac_deg_to_rad(app->stick.yaw)) * cosf(ac_deg_to_rad(app->stick.pitch))
            };
            new_velocity = ac_vec3_normalize(&new_velocity);
            new_velocity = ac_vec3_scale(&new_velocity, delta_velocity);

            // apply to cue ball
            app->physics_world.velocities[app->stick.target] = new_velocity;
        }

        // reset stick power
        app->stick.power = 0.0f;
        app->stick.strike = false;
    }

    // phys_update(&app->physics_world, delta_time);
    glutPostRedisplay();
}

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

void app_render_callback( void )
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // update camera
    update_view_matrix(&app->main_camera);

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

    // glClear(GL_DEPTH_BUFFER_BIT);
    // draw_power_bar(app->stick.power);

    // glClear(GL_DEPTH_BUFFER_BIT);
    // glViewport(2 * width / 3, 2 * height / 3, width / 3, height / 3); // set the viewport to the bottom left corner
    // draw_mini_map();

    glutSwapBuffers();
}

void app_key_callback(unsigned char key, int x, int y)
{
    (void) x; // nullify unused x
    (void) y; // nullify unused y

    static const float movement_step = 0.5f;
    static const float rotation_step = 2.0f;
    switch (key)
    {
    case '=':
        app->main_camera.radius = ac_clamp(app->main_camera.radius - movement_step, 10.0f, 100.0f);
        break;
    case '-':
        app->main_camera.radius = ac_clamp(app->main_camera.radius + movement_step, 10.0f, 100.0f);
        break;
    case 's':
    case 'S':
        app->stick.power = ac_clamp(app->stick.power + 0.01f, 0.0f, 1.0f);
        break;
    case 'w':
    case 'W':
        app->stick.power = ac_clamp(app->stick.power - 0.01f, 0.0f, 1.0f);
        break;
    case 'a':
    case 'A':
        app->stick.yaw += rotation_step;
        if (app->stick.yaw >= 360.0f)
        {
            app->stick.yaw -= 360.0f;
        }
        break;
    case 'd':
    case 'D':
        app->stick.yaw -= rotation_step;
        if (app->stick.yaw < 0.0f)
        {
            app->stick.yaw += 360.0f;
        }
        break;
    case 'q':
    case 'Q':
        app->stick.pitch = ac_clamp(app->stick.pitch + rotation_step, 0.0f, 90.0f);
        break;
    case 'e':
    case 'E':
        app->stick.pitch = ac_clamp(app->stick.pitch - rotation_step, 0.0f, 90.0f);
        break;
    case ' ':
        app->stick.strike = true;
        break;
    }
}

void app_special_key_callback(int key, int x, int y)
{
    (void) x; // nullify unused x
    (void) y; // nullify unused y

    static const float rotation_step = 2.0f;

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
    default:
        break;
    }
}