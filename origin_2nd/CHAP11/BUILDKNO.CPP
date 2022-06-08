
void buildKnots(int m, int L, double knot[])
{
	// Build the standard knot vector for L+1 control points 	
	// and B-splines of order m
	int i;
	if(L < (m - 1)) return;         // too few control points
	for(i = 0; i <= L + m; i++)
	   if (i < m) knot[i] = 0.0;
	   else if (i <= L) knot[i] = i - m + 1;    // i is at least m here 
	   else knot[i] = L - m + 2;       // i exceeds L here
}
