
void myDisplay(void)
{
	glBegin(GL_LINE_STRIP);
   for(GLfloat x = -4.0; x < 4.0; x += 0.1)
	{
		GLfloat y = sin(3.14159 * x) / (3.14159 * x);
		glVertex2f(x, y);
	}		
	glEnd();
	glFlush();
}

