void drawFlurry(int num, int numColors, int Width, int Height)
// draw num random rectangles in a Width by Height rectangle 
{
	for (int i = 0; i < num; i++) 
	{
		 GLint x1 = random(Width);			// place corner randomly 
		 GLint y1 = random(Height);
		 GLint x2 = random(Width); 		// pick the size so it fits 
		 GLint y2 = random(Height);
		 GLfloat lev = random(10)/10.0;		// random value, in range 0 to 1 
		 glColor3f(lev,lev,lev);			// set the gray level 
		 glRecti(x1, y1, x2, y2);			// draw the rectangle
	}
  glFlush();

}  
