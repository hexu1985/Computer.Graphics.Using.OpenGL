double MinLenSq, factor; // global variables 
#define SQR(a) ((a)*(a))

void fract(Point2 A, Point2 B, double  StdDev)
{ // generate a fractal curve from A to B.
	double r, LenSq;
	Point2 C;

	LenSq = SQR(A.x - B.x)) + SQR(A.y - B.y);
	if(LenSq  < MinLenSq) 
		lineTo(B); 
	else 
	{
		StdDev *= factor;        // scale StdDev by factor
		r = Gauss * StdDev;      // get random parameter value 
		C.x = 0.5 * (A.x + B.x) - r * (B.y - A.y);
		C.y = 0.5 * (A.y + B.y) + r * (B.x - A.x);
		fract(A, C,  StdDev);
		fract(C, B, StdDev);
	}
} 
