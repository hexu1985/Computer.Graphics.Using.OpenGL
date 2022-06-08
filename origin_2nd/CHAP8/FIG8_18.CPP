glBegin(GL_POLYGON);
for(int i = 0; i < 3; i++)
{
   glNormal3f(norm[i].x, norm[i].y, norm[i].z);
   glVertex3f(pt[i].x, pt[i].y, pt[i].z);
}
glEnd();
