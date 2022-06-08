bool TaperedCylinder::hit(Ray &r, Intersection &inter)
{ 
	Ray genRay; //generic ray
	xfrmRay(genRay,invTransf,r);
	double A, B, C, discrim, disc_root, t1,t2,tb,tc;
	double sm = smallRadius - 1;
	double fDir = sm * genRay.dir.z; //handy short names
	double fStart = sm * genRay.start.z + 1;
    A = SQR(genRay.dir.x)+ SQR(genRay.dir.y)- SQR(fDir);
   B = genRay.dir.x*genRay.start.x + genRay.dir.y*genRay.start.y -fDir*fStart;
   C = SQR(genRay.start.x)+ SQR(genRay.start.y)- SQR(fStart);
…..
