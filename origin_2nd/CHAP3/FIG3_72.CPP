// draw the curve (x(t), y(t)) using 
// the array t[0],..,t[n-1] of “sample-times”

glBegin(GL_LINES);
   for(int i = 0; i < n; i++)
      glVertex2f((x(t[i]), y(t[i]));
glEnd();

