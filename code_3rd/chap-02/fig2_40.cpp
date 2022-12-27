//#include <windows.h>   // use as needed for your system
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

struct GLintPoint {
    GLint x;
    GLint y;
};

const int screenWidth = 640;       // width of screen window in pixels 
const int screenHeight = 480;       // height of screen window in pixels

//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
void myInit(void)
{
    glClearColor(1.0,1.0,1.0,0.0);       // set white background color
    glColor3f(0.0f, 0.0f, 0.0f);          // set the drawing color 
    glPointSize(4.0);               // is 4 by 4 pixels
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);
}

//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);     // clear the screen 
    glFlush();                        // send all output to display 
}

//<<<<<<<<<<<<<<<<<<<<<<<< myMouse >>>>>>>>>>>>>>>>>
void myMouse(int button, int state, int x, int y)
{
    static GLintPoint corner[2];
    static int numCorners = 0;

    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            corner[numCorners].x = x;
            corner[numCorners].y = screenHeight - y;
            if (++numCorners == 2) {
                glRecti(corner[0].x, corner[0].y, corner[1].x, corner[1].y);
                numCorners = 0;
                glFlush();
            }
        }
    } else if (button == GLUT_RIGHT_BUTTON) {
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
    }
}

//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char** argv)
{
    glutInit(&argc, argv);          // initialize the toolkit
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
    glutInitWindowSize(screenWidth, screenHeight);     // set window size
    glutInitWindowPosition(100, 150); // set window position on screen
    glutCreateWindow("my first attempt"); // open the screen window
    glutDisplayFunc(myDisplay);     // register redraw function
    glutMouseFunc(myMouse);
    myInit();                   
    glutMainLoop();              // go into a perpetual loop

    return 0;
}
