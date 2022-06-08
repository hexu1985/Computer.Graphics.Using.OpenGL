
void Sierpinski(void) 
{
	GLintPoint T[3]= {{10,10},{300,30},{200, 300}};
	
	int index = random(3);         // 0, 1, or 2 equally likely 
	GLintPoint point = T[index]; 	 // initial point 
	drawDot(point.x, point.y);     // draw initial point 
	for(int i = 0; i < 1000; i++)  // draw 1000 dots
	{
		 index = random(3); 	
		 point.x = (point.x + T[index].x) / 2;
		 point.y = (point.y + T[index].y) / 2;
		 drawDot(point.x,point.y);  
	} 
	glFlush(); 	
}
