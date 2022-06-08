void parameterizedHouse(GLintPoint peak, GLint width, GLint height)
 // the top of house is at the peak; the size of house is given
 //  by height and width
{
	glBegin(GL_LINE_LOOP);
	  glVertex2i(peak.x,             peak.y);  // draw shell of house 
	  glVertex2i(peak.x + width / 2, peak.y - 3 * height /8);
	  glVertex2i(peak.x + width / 2  peak.y -     height);
	  glVertex2i(peak.x - width / 2, peak.y -     height);
	  glVertex2i(peak.x - width / 2, peak.y - 3 * height /8);	
	glEnd();
	draw chimney in the same fashion
	draw the door
	draw the window
}
