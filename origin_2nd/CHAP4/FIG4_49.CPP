int chopCI(double& tIn, double& tOut, double numer, double denom)
{
	double tHit;
	if (denom < 0) 		   // ray is entering 
	{
		tHit = numer / denom;
		if (tHit > tOut) return 0;  // early out
		else if (tHit > tIn) tIn = tHit; // take larger t
	}
	else if(denom > 0)          // ray is exiting
	{
		tHit = numer / denom;
		if(tHit < tIn) return 0;      // early out
		if(tHit < tout) tOut = tHit; // take smaller t
	}
	else             // denom is 0: ray is parallel 
	if(numer <= 0) return 0;   // missed the line

	return 1;	// CI is still non-empty
}
