bool rayHitsBoxExtent(Ray& r, Cuboid& cub)
bool Cube:: hit(Ray& r, Intersection& inter)
{		
		double tHit, numer, denom;
		double tIn = -100000.0, tOut = 100000.0;// plus-minus infinity
		int inSurf, outSurf; // which of the six surfaces
	
		for (int i = 0; i < 6; i++)
		{
		  switch(i)  // which plane of cube to test 
		  {
			 case 0: 
				numer = cub.top*(cub.top - genRay.start.y); 
				denom = cub.top*r.dir.y; 		break; 
			 case 1: 
				numer = cub.bottom *(cub.bottom - genRay.start.y); 
				denom = cub.bottom *r.dir.y; 
				break; 
					..
			 case 5: 
				numer = cub.back *(cub.back - genRay.start.z); 
				denom = cub.back *r.dir.z; 
				break; 
		  }
			if(fabs(denom) < 0.00001)    // ray is parallel 				
				if(numer < 0) return false; //ray is out;
				else;                 // ray inside, no change to tIn,tOut
			else                     // ray is not parallel
			{									
				tHit = numer / denom;
				if(denom > 0){		// exiting
					 if(tHit < tOut) // a new earlier exit
						tOut = tHit;
				} 
				else {		// denom is negative: entering 
					 if(tHit > tIn) // a new later entrance
						tIn = tHit;
					   }
			}
  		if(tIn >= tOut) return false; // it's a miss - early out
		} // end of the for loop 
		
		int num = 0; // no positive hits yet
		if(tIn > 0.00001) //is first hit in front of the eye?
					num++; //have a hit
		if(tOut > 0.00001)	num++;
		
		return (num > 0);
}