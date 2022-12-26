#include <windows.h>
#include <gl/Gl.h>
#include <gl/Glu.h>
#include <gl/Glut.h>
#include <math.h>

#include "RGBpixmap.h"

RGBpixmap pic[2]; // create two (empty) global pixmaps

int screenWidth = 640, screenHeight = 480;

IntPoint rasPosition(10,10);

int whichPic = 0; // which pixmap to display

//<<<<<<<<<<<<<<<<<<<<<<<<< myMouse >>>>>>>>>>>>>>>>>>>>>>>>
void myMouse(int button, int state, int mx, int my)
{ // set raster position with a left click	
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{	
		rasPosition.x = mx; rasPosition.y = screenHeight - my;
		glRasterPos2i(rasPosition.x, rasPosition.y);
		glutPostRedisplay();
	}
	else {
		glClear(GL_COLOR_BUFFER_BIT);
		// clear with right click
	}
}
//<<<<<<<<<<<<<<<<<<<<<<<<< mouseMove >>>>>>>>>>>>>>>>>
void mouseMove(int x, int y)
{// set raster position with mouse motion
	rasPosition.x = x; rasPosition.y = screenHeight - y;
	glRasterPos2i(rasPosition.x, rasPosition.y);
	pic[whichPic].draw();
	//glutPostRedisplay();
}
//<<<<<<<<<<<<<<<<<<<<<<<<<< myReshape >>>>>>>>>>>>>>>>>>>
void myReshape(int w, int h)
{
	screenWidth = w; screenHeight = h;
	pic[whichPic].draw();
	glutPostRedisplay();
}
//<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>>>>>>>>>>
void myDisplay(void)
{	
	pic[whichPic].draw(); //draw it at the raster position
	glFlush();
} 
//<<<<<<<<<<<<<<<<<<<<<<<< myKeys >>>>>>>>>>>>>>>>>>>>>>
void myKeys(unsigned char key, int x, int y)
{
	switch(key)
	{
	   case 'q': exit(0); break;
	   case 's': whichPic = 1 - whichPic; break; // switch pixmaps
	   case 'r': pic[whichPic].read(rasPosition.x, rasPosition.y,125,125); break; //grab a piece
	   case 'b': pic[0].draw(); break; // switch pixmaps
	}
	glutPostRedisplay();
}
//<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>>>>
void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(10, 30);
	glutCreateWindow("Experiment with Images");
	glutKeyboardFunc(myKeys);
	glutMouseFunc(myMouse);
	glutMotionFunc(mouseMove);
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glClearColor(1.0f, 0.0f, 0.0f, 0.0); //background color
	glClear(GL_COLOR_BUFFER_BIT);	
	pic[0].readBMPFile("man.bmp"); //make a pixmap
	pic[1].readBMPFile("red.bmp"); // make another one
	glutMainLoop();
}
