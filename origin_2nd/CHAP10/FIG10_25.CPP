bresenham(IntPoint a, IntPoint b)
{ // restriction: a.x < b.x and 0 < H/W < 1 
	int y = a.y, W = b.x - a.x, H = b.y - a.y;
	int F =  2 * H - W;    // current error term
	for(int x = a.x; x <= b.x; x++) // inner loop 
	{
		setPixel(x, y);
		if(F< 0) 
			F += 2 * H;      // set up for next pixel
		else{
			y++;
			F += 2 * (H - W);
		}
	}
} 
