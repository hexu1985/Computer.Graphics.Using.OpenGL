void myMouse(int button, int state, int x, int y)
{
	#define NUM 20
	static GLintPoint List[NUM];
    static int last = -1;            	// last index used so far 

   // test for mouse button as well as for a full array
   if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && last < NUM -1)
		{        
			  List[++last].x = x; 		    // add new point to list
		   List[  last].y = screenHeight - y; // window height is 480
		   glClear(GL_COLOR_BUFFER_BIT);	    // clear the screen 
		   glBegin(GL_LINE_STRIP); 		    // redraw the polyline				     for(int i = 0; i <= last; i++)
			     glVertex2i(List[i].x, List[i].y);
		   glEnd();
		   glFlush();
	}
	else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
last = -1;	         // reset the list to empty
}
