#include "GL/freeglut.h"
#include "src/app.h"

//--------------------------------------------------------------------------------------------------
// Forward Declatations
//--------------------------------------------------------------------------------------------------

void set_projection_matrix( int, int );
void resize_window( int, int );
void opengl_init( void );

//--------------------------------------------------------------------------------------------------
// Main
//--------------------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
    // GLUT Initialisation
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutInitWindowPosition(400, 100);
    glutCreateWindow("ace demo");
    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);

    // Local OpenGL Callbacks
    glutReshapeFunc(resize_window);

    // Application Callbacks
    glutDisplayFunc(app_render_callback);
    glutKeyboardFunc(app_key_callback);
    glutSpecialFunc(app_special_key_callback);

    // initialisation
    opengl_init();
    frame_time* timer = app_init();
    glutTimerFunc( 1000 / timer->update_rate, app_update_callback, 0);
    timer->last_frame_time = glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop();
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

void set_projection_matrix(int width, int height)
{
	GLdouble fov     = 38.0;    // degrees
	GLdouble aspect  = 1.0 * ((GLdouble) width / (GLdouble) height);
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
