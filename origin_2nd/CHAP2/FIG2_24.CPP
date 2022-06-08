

void hardwiredHouse(void)
{
	glBegin(GL_LINE_LOOP);
		glVertex2i(40, 40);	// draw the shell of house 
		glVertex2i(40, 90);
		glVertex2i(70, 120);
		glVertex2i(100, 90);
		glVertex2i(100, 40);
	glEnd();
	glBegin(GL_LINE_STRIP); 
		glVertex2i(50, 100);	// draw the chimney 
		glVertex2i(50, 120);
		glVertex2i(60, 120);
		glVertex2i(60, 110);
	glEnd();
		// draw the door 
		// draw the window 
}
