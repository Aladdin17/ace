#include "GL/freeglut.h"
#include "app.h"

// void draw_mini_map(void)
// {
//     glPushAttrib(GL_LIGHTING);
//     glDisable(GL_LIGHTING);

//     // set the projection matrix
//     glMatrixMode(GL_PROJECTION);
//     glPushMatrix();
//     glLoadIdentity();
//     glOrtho(
//         -25, 25,
//         -15, 15,
//         0, 10
//     );
//     glMatrixMode(GL_MODELVIEW);
//     glPushMatrix();
//     glLoadIdentity();
//     gluLookAt(
//         0, 10, 0,
//         0, 0, 0,
//         -1, 0, 0
//     );
//     render_scene();
//     glPopMatrix();

//     // restore the projection matrix
//     glMatrixMode(GL_PROJECTION);
//     glPopMatrix();

//     glPopAttrib();
// }

// void draw_power_bar(float percentage)
// {
//     // disable depth test for this overlay
//     glPushAttrib(GL_DEPTH_TEST | GL_LIGHTING);
//     glDisable(GL_DEPTH_TEST);
//     glDisable(GL_LIGHTING);

//     // window dimensions
//     int width = glutGet(GLUT_WINDOW_WIDTH);
//     int height = glutGet(GLUT_WINDOW_HEIGHT);

//     // set the projection matrix
//     glMatrixMode(GL_PROJECTION);
//     glPushMatrix();
//     glLoadIdentity();
//     gluOrtho2D(0.0, width, 0.0, height);

//     glMatrixMode(GL_MODELVIEW);
//     glPushMatrix();
//     glLoadIdentity();

//     // calculate width of the power bar
//     float power_bar_max_width = (float) width / 4.0f;
//     float power_bar_height = (float) height / 20.0f;

//     // draw outline
//     glColor3f(1.0f, 1.0f, 1.0f);
//     glRectf(0.0f, 0.0f, power_bar_max_width, power_bar_height);

//     // draw power bar
//     glColor3f(1.0f, 0.0f, 0.0f);
//     glRectf(0.0f, 0.0f, power_bar_max_width * percentage, power_bar_height);

//     // render text
//     static const char* text = "Power";
//     glColor3f(1.0f, 0.0f, 0.0f);
//     glRasterPos2f(power_bar_max_width + 10.0f, power_bar_height / 2.0f);
//     for (int i = 0; text[i] != '\0'; i++)
//     {
//         glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
//     }

//     glPopMatrix();

//     glMatrixMode(GL_PROJECTION);
//     glPopMatrix();

//     // restore depth test
//     glPopAttrib();
// }

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
    glutReshapeFunc(resize_window);
    glutDisplayFunc(app_render_callback);
    glutKeyboardFunc(app_key_callback);
    glutSpecialFunc(app_special_key_callback);

    // initialisation
    opengl_init();
    frame_time* timer = app_init();
    glutTimerFunc( 1000 / timer->update_rate, app_update, 0);
    timer->last_frame_time = glutGet(GLUT_ELAPSED_TIME);
    glutMainLoop();
}
