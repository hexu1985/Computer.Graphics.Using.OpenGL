IntRect rr; // global rectangle

//<<<<<<<<<<<<<<<< myMouse >>>>>>>>>>>>>>>>
void myMouse(int button, int state, int mx, int my)
{       
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{       
			glEnable(GL_COLOR_LOGIC_OP); //enable logical operations
			glLogicOp(GL_XOR);           // set it to XOR mode
			rr.left = rr.right = mx; //set the pivot
			rr.top = rr.bott = screenHeight – my;
	}
	if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		glDisable(GL_COLOR_LOGIC_OP); // disable logical operations
}
//<<<<<<<<<<<<<<<< mouseMove >>>>>>>>>>>>>>>>>
void mouseMove(int mx, int my)
{
		rr.draw(); //erase the old: works only in XOR mode
		rr.right = mx; // set the new opp. corner
		rr.bott = screenHeight - my; // flip y-coord.
		rr.draw(); // draw the new
}
