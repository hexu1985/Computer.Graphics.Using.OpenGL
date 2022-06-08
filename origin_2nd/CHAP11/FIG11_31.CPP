float bSpline(int k, int m, float t, float knot[])
{
	float denom1, denom2, sum = 0.0;
	if(m == 1)
	   return (t >= knot[k] && t < knot[k+1]); // 1 or 0
	// m exceeds 1.. recurse
	denom1 = knot[k + m -1] - knot[k];
	if(denom1 != 0.0)
		sum = (t - knot[k]) * bSpline(k,m-1,t, knot) / denom1;
	denom2 = knot[k + m] - knot[k+1];
	if(denom2 != 0.0)
		sum += (knot[k+m] - t) * bSpline(k+1,m-1,t,knot) / denom2;
	return sum;
}
