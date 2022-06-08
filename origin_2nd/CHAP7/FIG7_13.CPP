// the usual includes
#include "camera.h"

Camera cam; // global camera object

//<<<<<<<<<<<<<<<<<<<<<<<< myKeyboard >>>>>>>>>>>>>>>>>>>>>>
void myKeyboard(unsigned char key, int x, int y)
{
  switch(key)
  {	
	// controls for camera
	case 'F':    cam.slide(0,0, 0.2); break; // slide camera forward
	case 'F'-64: cam.slide(0,0,-0.2); break; //slide camera back	
	// add up/down and left/right controls	
	case 'P':      cam.pitch(-1.0); break;
	case 'P' - 64: cam.pitch( 1.0); break;
	// add roll and yaw controls
  }
	glutPostRedisplay(); // draw it again
}
//<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>>>>>>>>>>
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT||GL_DEPTH_BUFFER_BIT);	
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
	glClearColor(1.0f,1.0f,1.0f,1.0f);  // background is white 
	glColor3f(0.0f,0.0f,0.0f);			// set color of stuff 
	glViewport(0, 0, 640, 480);
	cam.set(4, 4, 4, 0, 0, 0, 0, 1, 0); // make the initial camera
	cam.setShape(30.0f, 64.0f/48.0f, 0.5f, 50.0f);
	glutMainLoop();
}
