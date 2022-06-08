void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
	  GLint x = mouseX;
  GLint y = screenHeight - mouseY; // flip the y value as always
	  switch(theKey)
  {
		  case ‘p’:
		  	drawDot(x, y);	// draw a dot at the mouse position
		  	break;
	  	case GLUT_KEY_LEFT: List[++last].x = x; // add a point
                        List[  last].y = y;
			  break;
    case ‘E’:
			  exit(-1);		//terminate the program		
	  	default: 
			break;		      // do nothing
	}
}
