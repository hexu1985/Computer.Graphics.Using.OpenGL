#include "windows.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include "camera.h"


Camera cam; // make a global camera object

//<<<<<<<<<<<<<<<<<<<<<<<< myKeyboard >>>>>>>>>>>>>>>>>>>>>>
void myKeyboard(unsigned char key, int x, int y)
{
  switch(key)
  {	

	  // controls for camera
	case 'L':      cam.slide(.2, 0, 0); break;// slide camera right
	case 'L' - 64: cam.slide(-0.2, 0, 0); break; // slide camera left

	case 'U':      cam.slide(0, .2, 0); break;// slide camera up
	case 'U' - 64: cam.slide(0, -0.2, 0); break; // slide camera down

	case 'F':    cam.slide(0,0, 0.2); break; // slide camera forward
	case 'F'-64: cam.slide(0,0,-0.2); break; //slide camera back	
	// add up/down and left/right controls	
	case 'P':      cam.pitch(-1.0); break;
	case 'P' - 64: cam.pitch( 1.0); break;
	// add yaw controls
	case 'Y':      cam.yaw(-1.0); break;
	case 'Y' - 64: cam.yaw( 1.0); break;
	// add roll controls
	case 'R':      cam.roll(1.0); break;
	case 'R' - 64: cam.roll(-1.0); break;
  }
	glutPostRedisplay(); // draws it again
}

//<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>>>>>>>>>>
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glutWireTeapot(1.0); // draw the teapot
	glFlush();
	glutSwapBuffers(); // display the screen just made
}
 
//<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // double buffering
	glutInitWindowSize(640,480);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("fly a camera around a teapot");
	glutKeyboardFunc(myKeyboard);
	glutDisplayFunc(myDisplay);
	glClearColor(1.0f,0.0f,0.0f,0.0f);  // background is red 
	glColor3f(0.0f,0.0f,0.0f);			// set color of stuff 
	glViewport(0, 0, 640, 480);
	cam.set(4, 4, 4, 0, 0, 0, 0, 1, 0); // make the initial camera
	cam.setShape(30.0f, 64.0f/48.0f, 0.5f, 50.0f);
	glutMainLoop();
}