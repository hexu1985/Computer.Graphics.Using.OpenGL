//additional methods for Canvas class
//components of 2D transformation
//<<<<<<<<<<<<<<< initCT >>>>>>>>>>>>>>>>>
void Canvas:: initCT(void)			          
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();        // set CT to the identity matrix
}
//<<<<<<<<<<<<<< scale2D >>>>>>>>>>>>>>>>>>>>
void Canvas:: scale2D(double sx, double sy) 
{
	glMatrixMode(GL_MODELVIEW);
	glScaled(sx, sy, 1.0); // set CT to CT * (2D scaling)

}
//<<<<<<<<<<<<<<< translate2D >>>>>>>>>>>>>>>>>
void Canvas:: translate2D(double dx, double dy)     
{	glMatrixMode(GL_MODELVIEW);
	glTranslated(dx, dy, 1.0); // set CT to CT * (2D translation)
}
//<<<<<<<<<<<<<<<< rotate2D >>>>>>>>>>>>>>>>>>>>
void Canvas:: rotate2D(double angle)        
{
	glMatrixMode(GL_MODELVIEW);
	glRotated(angle, 0.0, 0.0, 1.0); // set CT to CT * (2D rotation)
}

