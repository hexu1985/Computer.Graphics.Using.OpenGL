//#include <windows.h>   // use as needed for your system
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

int random(int n)
{
    return rand()%n;
}

void drawFlurry(int num, int numColors, int Width, int Height)
    // draw num random rectangles in a Width by Height rectangle 
{
    for (int i = 0; i < num; i++) 
    {
        GLint x1 = random(Width);			// place corner randomly 
        GLint y1 = random(Height);
        GLint x2 = random(Width); 		// pick the size so it fits 
        GLint y2 = random(Height);
        GLfloat lev = random(10)/10.0;		// random value, in range 0 to 1 
        glColor3f(lev,lev,lev);			// set the gray level 
        glRecti(x1, y1, x2, y2);			// draw the rectangle
    }
    glFlush();
}  

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
    glClear(GL_COLOR_BUFFER_BIT);     // clear the screen 
    drawFlurry(10, 10, 640, 480);
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

