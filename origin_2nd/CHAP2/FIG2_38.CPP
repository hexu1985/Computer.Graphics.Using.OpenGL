	void myMouse(int button, int state, int x, int y)
{
	 static GLintPoint corner[2];
    static int numCorners = 0;		         // initial value is 0
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	 {
		  corner[numCorners].x = x;
			 corner[numCorners].y = screenHeight - y;    // flip y coordinate
		  numCorners++;		                     // have another point
		  if(numCorners == 2)
		  {

		    glRecti(corner[0].x, corner[0].y, corner[1].x, corner[1].y);
			    numCorners = 0;				   // back to 0 corners
		  }
	 }
	 else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	glClear(GL_COLOR_BUFFER_BIT);	         // clear the window
	 glFlush();
}
