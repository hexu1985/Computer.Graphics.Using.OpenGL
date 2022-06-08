#include <windows.h>
#include <GL\GL.h>

class GLintPoint{
public: 
		GLint x, y;
};


GLintPoint CP;         // global current position

//<<<<<<<<<<<<< moveto >>>>>>>>>>>>>>
void moveto(GLint x, GLint y)
{
	CP.x = x; CP.y = y; // update the CP 
}
//<<<<<<<<<<<< lineTo >>>>>>>>>>>>>>>>>
void lineto(GLint x, GLint y)
{
	glBegin(GL_LINES);  // draw the line 
		glVertex2i(CP.x, CP.y);
		glVertex2i(x, y);
	glEnd();
	glFlush();
	CP.x = x; CP.y = y; // update the CP 
}
