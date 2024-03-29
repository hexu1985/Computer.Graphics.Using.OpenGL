#include <windows.h>
#include <iostream.h>
#include <math.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/GLUT.h>

// use a named constant to approximate pi
const float pi = 3.14159265358979;

//--------------- setWindow ---------------------
// define our own function to specify the window, as explained later
void setWindow(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top)

{
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(left, right, bottom, top);
}


//---------------- setViewport ------------------
void setViewport(int left, int right, int width, int height)
// define our own function to specify the viewport, as explained later
{
            GLfloat bottom = 0.5;
            GLfloat top = 1.5;
            glViewport(left, bottom, right - left, top - bottom);
}

//<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void myDisplay(void) // plot the sinc function, using world coordinates

{
		glClear( GL_COLOR_BUFFER_BIT );
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glBegin(GL_LINE_STRIP);
/* automatically uses the defined window and viewport, 
clips properly, and performs the proper mapping */

		for(
GLfloat x = -4.0; x < 4.0; x += 0.1)     // draw the plot
                   glVertex2f(x, sin(pi * x) / (pi * x));

            glEnd();
            glFlush();
}

 

//<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void myInit(void)

{
            glClearColor(1.0,1.0,1.0,0.0);
            glColor3f(0.0f,0.0f,1.0f);
            glLineWidth(1.0);
}

//<<<<<<<<<<<<<<main>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void main(int argc, char** argv)

{
            glutInit(&argc, argv);
            glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
            glutInitWindowSize(640,480);
            glutInitWindowPosition(100,150);
            glutCreateWindow("Sinc Function");
            glutDisplayFunc(myDisplay);
            myInit( );
			// call the function to specify the window
		    setWindow(-5.0, 5.0, -0.3, 1.0);
            // call the function to specify the viewport
			setViewport(0, 640, 0, 480);
			glutMainLoop( );
}
