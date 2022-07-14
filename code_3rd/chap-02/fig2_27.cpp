//#include <windows.h>   // use as needed for your system
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

struct GLintPoint {
    GLint x;
    GLint y;
};

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

void parameterizedHouse(GLintPoint peak, GLint width, GLint height)
 // the top of house is at the peak; the size of house is given
 //  by height and width
{
	glBegin(GL_LINE_LOOP);
	  glVertex2i(peak.x,             peak.y);  // draw shell of house 
	  glVertex2i(peak.x + width / 2, peak.y - 3 * height / 8);
	  glVertex2i(peak.x + width / 2, peak.y -     height);
	  glVertex2i(peak.x - width / 2, peak.y -     height);
	  glVertex2i(peak.x - width / 2, peak.y - 3 * height / 8);	
	glEnd();
	glBegin(GL_LINE_STRIP); 
		glVertex2i(peak.x - width / 3, peak.y - height / 4);	// draw the chimney 
		glVertex2i(peak.x - width / 3, peak.y);
		glVertex2i(peak.x - width / 6, peak.y);
		glVertex2i(peak.x - width / 6, peak.y - height / 8);
	glEnd();
	glBegin(GL_LINE_STRIP);
		glVertex2i(peak.x - width / 3, peak.y -     height);	// draw the door 
		glVertex2i(peak.x - width / 3, peak.y - 9 * height / 16);
		glVertex2i(peak.x - width / 12, peak.y - 9 * height / 16);
		glVertex2i(peak.x - width / 12, peak.y -     height);
	glEnd();
	glBegin(GL_LINE_LOOP);
		glVertex2i(peak.x + width / 12, peak.y - 5 * height / 8);    // draw the window 
		glVertex2i(peak.x + width / 12, peak.y - 7 * height / 16);	
		glVertex2i(peak.x + width / 3, peak.y - 7 * height / 16);
		glVertex2i(peak.x + width / 3, peak.y - 5 * height / 8);
	glEnd();
}

//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);     // clear the screen 
    parameterizedHouse(GLintPoint{70,120}, 60, 80);
    parameterizedHouse(GLintPoint{90,200}, 30, 40);
    parameterizedHouse(GLintPoint{200,200}, 90, 60);
    parameterizedHouse(GLintPoint{300,200}, -90, -60);
    parameterizedHouse(GLintPoint{70,250}, 60, -80);
    parameterizedHouse(GLintPoint{400,200}, 60, 120);
    parameterizedHouse(GLintPoint{400,300}, -90, 60);
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
