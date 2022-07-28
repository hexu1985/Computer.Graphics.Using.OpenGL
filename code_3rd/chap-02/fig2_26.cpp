//#include <windows.h>   // use as needed for your system
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

void hardwiredHouse(void)
{
	glBegin(GL_LINE_LOOP);
		glVertex2i(40, 40);	// draw the shell of house 
		glVertex2i(40, 90);
		glVertex2i(70, 120);
		glVertex2i(100, 90);
		glVertex2i(100, 40);
	glEnd();
	glBegin(GL_LINE_STRIP); 
		glVertex2i(50, 100);	// draw the chimney 
		glVertex2i(50, 120);
		glVertex2i(60, 120);
		glVertex2i(60, 110);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex2i(50, 40);	// draw the door 
		glVertex2i(50, 75);
		glVertex2i(65, 75);
		glVertex2i(65, 40);
	glEnd();
	glBegin(GL_LINE_LOOP);
		glVertex2i(75, 70);    // draw the window 
		glVertex2i(75, 85);	
		glVertex2i(90, 85);
		glVertex2i(90, 70);
	glEnd();
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
    hardwiredHouse();
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
