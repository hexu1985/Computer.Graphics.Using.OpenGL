
	for(float i=0; i<20; i+=0.01)
	{
		point.cx = 30*i+50;
		point.cy = 50 * cos(i) +200;
		drawDot(point);
	}