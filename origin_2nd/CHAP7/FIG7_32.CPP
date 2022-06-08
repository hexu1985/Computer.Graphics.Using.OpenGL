int clipEdge(Point4& A, Point4& C)
{
	double tIn = 0.0, tOut = 1.0, tHit;
	double aBC[6], cBC[6];
	int aOutcode = 0, cOutcode = 0;	
	<.. find BC’s for A and C ..>
	<.. form outcodes for A and C ..>

	if((aOutcode & cOutcode) != 0) // trivial reject
			return 0; 
	if((aOutcode | cOutcode) == 0) // trivial accept
			return 1;

	for(int i = 0; i < 6; i++) // clip against each plane 
	{				
		if(cBC[i] < 0)  // exits: C is outside
		{
			tHit = aBC[i]/(aBC[i] - cBC[i]);
			tOut = MIN(tOut,tHit);
		}
		else if(aBC[i] < 0) //enters: A is outside
		{
			tHit = aBC[i]/(aBC[i] - cBC[i]);
			tIn = MAX(tIn, tHit);
		}
		if(tIn > tOut) return 0; //CI is empty early out
	}	 
	// update the end points as necessary 
	Point4 tmp;
	if(aOutcode != 0) // A is out: tIn has changed
	{ // find updated A, (but don’t change it yet)
		tmp.x = A.x + tIn * (C.x - A.x);
		tmp.y = A.y + tIn * (C.y - A.y);
		tmp.z = A.z + tIn * (C.z - A.z);
		tmp.w = A.w + tIn * (C.w - A.w);
	}
	if(cOutcode != 0) // C is out: tOut has changed
	{	// update C (using original value of A)
		C.x = A.x + tOut * (C.x - A.x);
		C.y = A.y + tOut * (C.y - A.y);
		C.z = A.z + tOut * (C.z - A.z);
		C.w = A.w + tOut * (C.w - A.w);
	}
	A = tmp;	// now update A
	return 1; // some of the edge lies inside the CVV
}
