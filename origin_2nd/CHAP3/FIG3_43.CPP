void ngon(int n, float cx, float cy, float radius, float rotAngle)
{      // assumes global Canvas object, cvs
	if(n < 3) return; 					// bad number of sides
   double angle = rotAngle * 3.14159265 / 180;  // initial angle
   double angleInc = 2 * 3.14159265 /n;         //angle increment
	cvs. moveTo(radius + cx, cy);
	for(int k = 0; k < n; k++)  // repeat n times
	{
		angle += angleInc; 
		cvs.lineTo(radius * cos(angle) + cx, radius * sin(angle) + cy);
	}
}
