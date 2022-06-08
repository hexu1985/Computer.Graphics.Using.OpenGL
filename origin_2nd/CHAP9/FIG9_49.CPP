int dwell(double cx, double cy)
  { // return true dwell or Num, whichever is smaller 
   #define Num 100 	 // increase this for better pictures

	double tmp, dx = cx, dy = cy, fsq = cx * cx + cy * cy;
	for(int count = 0; count <= Num && fsq <= 4; count++)
	{
		tmp = dx;			             // save old real part
		dx = dx * dx – dy * dy + cx;	  // new real part
		dy = 2.0 * tmp * dy + cy;       // new imag. part
		fsq = dx * dx + dy* dy;
	}								
	return count;		                 // number of iterations used
  } 
