bool Cone::hit(Ray &r, Intersection &inter)
{ 
	Ray genRay; //generic ray
	xfrmRay(genRay,invTransf,r);
	double A, B, C, discrim, disc_root, t1,t2,tb,tc;
	double sm = smallRadius - 1;
	double fDir = sm * genRay.dir.z; //handy short names
	double fStart = sm * genRay.start.z + 1;
    get A, B, and C as in Equation 14.16
	int num = 0; // no hits yet
			
	if(discrim > 0.0)	//can take square root
	{
		disc_root = (double)sqrt(double(discrim));
		t1 = (-B - disc_root)/A;         // earlier hit
		float zHit = genRay.start.z + genRay.dir.z * t1; //z component of ray
		if(t1 > 0.00001 && zHit <= 1.0 && zHit >= 0) 
		{
			inter.hit[num].hitTime = t1;
			inter.hit[num++].surface = 0; //hit is with wall 
		}
		t2 = (-B + disc_root)/A; 		// second hit
		zHit = genRay.start.z + genRay.dir.z * t2;
		if(t2 > 0.00001 && zHit <= 1.0 && zHit >=0)
		{
			inter.hit[num].hitTime = t2;
			inter.hit[num++].surface = 0; //hit is with wall		
		}
	} // end if(discrim > 0)
			
	//test the base at z = 0
	tb = -genRay.start.z/genRay.dir.z; //hit time at z = 0 plane
	if(tb > 0.00001 && SQR(genRay.start.x + genRay.dir.x * tb) + 
		SQR(genRay.start.y + genRay.dir.y * tb) < 1) //within disc of base
	{
		inter.hit[num].hitTime = tb;
		inter.hit[num++].surface = 1; //1 for the base
	}

	if(num == 0) return false; // missed everything, or behind the eye
	inter.numHits = num;
			
	if(num == 1) // eye inside cylinder, only have the exiting hit
	{
		inter.hit[0].isEntering = false;
		inter.hit[0].hitObject = this;
	}
else // have two hits - first must be entering
	{   //now sort the two hits
		if(inter.hit[0].hitTime > inter.hit[1].hitTime) // must reverse them
		{// need only swap the hitTime and surface fields
			double tmpT = inter.hit[0].hitTime; //swap times
			inter.hit[0].hitTime = inter.hit[1].hitTime;
			inter.hit[1].hitTime = tmpT;
			int tmpS = inter.hit[0].surface; //swap surfaces
			inter.hit[0].surface = inter.hit[1].surface;
			inter.hit[1].surface = tmpS;
		}
		inter.hit[0].isEntering = true; inter.hit[1].isEntering = false;				inter.hit[0].hitObject = inter.hit[1].hitObject = this;
	}
	// now set the hit point and normal for the hit or hits
	for(int i = 0; i < num; i++)
	{
		Point3 P0(rayPos(genRay, inter.hit[i].hitTime));
		// position of first hit
		inter.hit[i].hitPoint.set(P0);
		int surf = inter.hit[i].surface;
		if(surf == 0) // wall
			inter.hit[i].hitNormal.set(P0.x, P0.y, -sm * (1 + sm * P0.z));
		else if(surf == 1) // base
			  inter.hit[i].hitNormal.set(0,0,-1); 
	}			
		return true;
}
