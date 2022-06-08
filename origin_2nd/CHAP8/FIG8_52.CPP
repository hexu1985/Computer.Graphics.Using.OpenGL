// <… the usual includes …>
#include "RGBpixmap.h"
//######################## GLOBALS ########################
RGBpixmap pix[6];    // make six (empty) pixmaps
float xSpeed = 0, ySpeed = 0,  xAngle = 0.0, yAngle = 0.0;
//<<<<<<<<<<<<<<<<<<<<<<<<<<< myinit >>>>>>>>>>>>>>>>>>>>>>>>>>>.
void myInit(void)
{	
	glClearColor(1.0f,1.0f,1.0f,1.0f);  // background is white 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	pix[0].makeCheckerboard();        	// make pixmap procedurally 
pix[0].setTexture(2001);              	// create texture
	pix[1].readBMPFile("Mandrill.bmp");  // make pixmap from image
pix[1].setTexture(2002);		// create texture
//< …similarly for other four textures …>

	glViewport(0, 0, 640, 480); // set up the viewing system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 640.0/ 480, 1.0, 30.0); // set camera shape
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(0.0, 0.0, -4); // move camera back
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<< display >>>>>>>>>>>>>>>>>>>>>>
void display(void)
{
  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glPushMatrix();
	glRotated(xAngle, 1.0,0.0,0.0);  glRotated(yAngle, 0.0,1.0,0.0); // rotate
		
	glBindTexture(GL_TEXTURE_2D,2001); 	// top face: 'fake' checkerboard
	glBegin(GL_QUADS);
	glTexCoord2f(-1.0, -1.0); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(-1.0, 2.0); glVertex3f(-1.0f, 1.0f,  1.0f);
	glTexCoord2f(2.0, 2.0); glVertex3f( 1.0f, 1.0f,  1.0f);
	glTexCoord2f(2.0, -1.0); glVertex3f( 1.0f, 1.0f, -1.0f);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,2002); 	// right face: mandrill
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(1.0f, -1.0f,  1.0f);
	glTexCoord2f(0.0, 2.0); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(2.0, 2.0); glVertex3f(1.0f,  1.0f, -1.0f);
	glTexCoord2f(2.0, 0.0); glVertex3f(1.0f,  1.0f,  1.0f);
	glEnd();	

// <… similarly for other four faces …>
	glFlush();
	glPopMatrix();
	glutSwapBuffers();
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< spinner >>>>>>>>>>>>>>>>>>>>>
void spinner(void)
{ // alter angles by small amount
	xAngle += xSpeed;  yAngle += ySpeed;
	display();
} 
//<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640,480);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("rotating textured cube");
	glutDisplayFunc(display);
	myInit();
	glutIdleFunc(spinner);
	glutMainLoop();
} 
