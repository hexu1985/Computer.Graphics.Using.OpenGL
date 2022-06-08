bool rayHitsSphereExtent(Ray & ray, SphereInfo& sph)
{
	double A = dot3D(ray.dir, ray.dir);
	Vector3 diff = ray.start – sph.center;
	double B = dot3D(diff, ray.dir);
	double C = dot3D(diff,diff) - sph.radSq;
	return(B * B >= A * C);	
}