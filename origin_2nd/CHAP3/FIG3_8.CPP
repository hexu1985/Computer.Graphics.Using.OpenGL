void myDisplay(void) // plot the sinc function, using world coordinates
{
	setWindow(-5.0, 5.0, -0.3, 1.0);			// set the window
	setViewport(0, 640, 0, 480);			// set the viewport
	glBegin(GL_LINE_STRIP);
   for(GLfloat x = -4.0; x < 4.0; x += 0.1)     // draw the plot
		glVertex2f(x, sin(3.14159 * x) / (3.14159 * x)); 
	glEnd();
	glFlush();
}
