//#include <windows.h>   // use as needed for your system
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
void myInit(void)
{
    glClearColor(1.0,1.0,1.0,0.0);       // set white background color
    glColor3f(0.0f, 0.0f, 0.0f);          // set the drawing color 
    glPointSize(4.0);               // is 4 by 4 pixels
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
    glClearColor(1.0,1.0,1.0,0.0); // white background 
    glClear(GL_COLOR_BUFFER_BIT);  // clear the window
    glColor3f(0.6,0.6,0.6);		  // bright gray
    glRecti(20,20,100,70);
    glColor3f(0.2,0.2,0.2);		  // dark gray
    glRecti(70, 50, 150, 130);
    glFlush();
}

//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char** argv)
{
    glutInit(&argc, argv);          // initialize the toolkit
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
    glutInitWindowSize(640,480);     // set window size
    glutInitWindowPosition(100, 150); // set window position on screen
    glutCreateWindow("my first attempt"); // open the screen window
    glutDisplayFunc(myDisplay);     // register redraw function
    myInit();                   
    glutMainLoop();              // go into a perpetual loop

    return 0;
}
