drawArc(Point2 center, float radius, float startAngle, float sweep)
{ 	  // startAngle and sweep are in degrees 
   const int n = 30; // number of intermediate segments in arc 
	float angle = startAngle * 3.14159265 / 180; // initial angle in radians
	float angleInc = sweep * 3.14159265 /(180 * n); // angle increment
	float cx = center.getX(), cy = center.getY();
	cvs.moveTo(cx + radius * cos(angle), cy + radius * sin(angle));
   for(int k = 1; k < n; k++, angle += angleInc)
		cvs.lineTo(cx + radius * cos(angle), cy + radius * sin(angle));
}
