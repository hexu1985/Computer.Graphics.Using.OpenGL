//--------------- setWindow ---------------------
void setWindow(GLdouble left, Gldouble right, GLdouble bottom, GLdouble top)
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluOrtho2D(left, right, bottom, top); 
}
//---------------- setViewport ------------------
void setViewport(GLdouble left, Gldouble right, GLdouble bottom, GLdouble top)
{
glViewport(left, bottom, right – left, top - bottom);
}
