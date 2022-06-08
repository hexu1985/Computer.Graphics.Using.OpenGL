RGBpixmap pic[2]; // create two (empty) global pixmaps
 int screenWidth = 640, screenHeight = 480;
IntPoint rasterPos(100,100);
int whichPic = 0; // which pixmap to display
//<<<<<<<<<<<<<<<<<<<<<<<<< myMouse >>>>>>>>>>>>>>>>>>>>>>>>
void myMouse(int button, int state, int mx, int my)
{ // set raster position with a left click	
	if(button == GLUT_LEFT_BUTTON)
	{	
		rasterPos.x = mx; rasterPos.y = screenHeight - my;
		glRasterPos2i(rasterPos.x, rasterPos.y);
		glutPostRedisplay();
	}
	else glClear(GL_COLOR_BUFFER_BIT); // clear with right click
}
//<<<<<<<<<<<<<<<<<<<<<<<<< mouseMove >>>>>>>>>>>>>>>>>
void mouseMove(int x, int y)
{// set raster position with mouse motion
	rasterPos.x = x; rasterPos.y = screenHeight - y;
	glRasterPos2i(rasterPos.x, rasterPos.y);
	glutPostRedisplay();
}
//<<<<<<<<<<<<<<<<<<<<<<<<<< myReshape >>>>>>>>>>>>>>>>>>>
void myReshape(int w, int h)
{
screenWidth = w; screenHeight = h;
}
//<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>>>>>>>>>>
void myDisplay(void)
{	
	pic[whichPic].draw(); //draw it at the raster position
} 
//<<<<<<<<<<<<<<<<<<<<<<<< myKeys >>>>>>>>>>>>>>>>>>>>>>
void myKeys(unsigned char key, int x, int y)
{
	switch(key)
	{
	   case 'q': exit(0);
	   case 's': whichPic = 1 - whichPic; break; // switch pixmaps
	   case 'r': pic[0].read(0,0,200,200); break; //grab a piece
	}
	glutPostRedisplay();
}
//<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>>>>
void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(30, 30);
	glutCreateWindow("Experiment with images");
	glutKeyboardFunc(myKeys);
	glutMouseFunc(myMouse);
	glutMotionFunc(mouseMove);
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
  glClearColor(0.9f, 0.9f, 0.9f, 0.0); //background color
	glClear(GL_COLOR_BUFFER_BIT);	
  pic[0].readBmpFile("CokeCan2.bmp"); //make a pixmap
	pic[1].readBmpFile("Mandrill.bmp"); // make another one
	glutMainLoop();
}
