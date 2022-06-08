Point2 y(float t, float S, int seg)
{
	double t2 = t * t, t3 = t2 * t;
      return (seg == 0)?
	  (S-4) * t3 + (6 - S) * t2 + 1:
	  (S+1) * t3 -(2 * S + 1) * t2 + S * t + 3;
}
