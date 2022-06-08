void Camera:: setOblique(Vector3 d,..others..)
{ // establish camera for oblique projections
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(l,r,b,t,n,f);         // set the projection matrix
	if(d.z == 0.0) return;		// for orthographic projections
	float m[16]; // space for a 4 by 4 matrix
for(int i = 0; i < 16; i++)   // start with identity matrix
	    m[i] = (i%5 == 0)? 1.0 : 0.0;// identity matrix
	m[8] = -d.x/d.z;              // add the shear terms
	m[9] = -d.y/d.z;
	glMultMatrixf(m);             // postmultiply it by m
}

