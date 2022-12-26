//SDL.cpp, fsh, 11/4/99 adjusted 1/30/2000 for ECE661 to admit simple raytracing
// support code for the classes in SDL.h
// some pieces not implemented - left for students to do
#include "SDL.h"
#include "camera.h"
//<<<<<<<<<<<<<<<<<< dot3D >>>>>>>>>>>>>>>>
float dot3D(Vector3& a, Vector3& b){return a.x * b.x + a.y * b.y + a.z * b.z;}
float dot3D(Point3&  a, Vector3& b){return a.x * b.x + a.y * b.y + a.z * b.z;}
float dot3D(Point3&  a, Point3& b) {return a.x * b.x + a.y * b.y + a.z * b.z;}
//<<<<<<<<<<<<<<<<<<<< rayPos >>>>>>>>>>>>>>>>>
Point3 rayPos(Point3& s, Vector3& v, float t)
{
	Point3 p(s.x + v.x * t,
		s.y + v.y * t,
		s.z + v.z * t);
	return p;
}
//<<<<<<<<<<<<<<<< rayPos >>>>>>>>>>>>>>>>>>>>>..
Point3 rayPos(Ray&r, float t)
{
	Point3 p(r.start.x + r.dir.x * t,
		r.start.y + r.dir.y * t,
		r.start.z + r.dir.z * t);
	return p;
}
//<<<<<<<<<<<<<<<<<<<< xfrmPoint >>>>>>>>>>>...
void xfrmPoint(Point3& res, Affine4& aff,Point3& pt)
{ // form res = aff * pt - include translation part
	res.x = aff.m[0] * pt.x + aff.m[4] * pt.y + aff.m[8]  * pt.z + aff.m[12];
	res.y = aff.m[1] * pt.x + aff.m[5] * pt.y + aff.m[9]  * pt.z + aff.m[13];
	res.z = aff.m[2] * pt.x + aff.m[6] * pt.y + aff.m[10] * pt.z + aff.m[14];
}
//<<<<<<<<<<<<<<<<<<<< xfrmVect >>>>>>>>>>>...
void xfrmVect(Vector3& res, Affine4& aff,Vector3& v)
{ // form res = aff * v
	res.x = aff.m[0] * v.x + aff.m[4] * v.y + aff.m[8]  * v.z;
	res.y = aff.m[1] * v.x + aff.m[5] * v.y + aff.m[9]  * v.z;
	res.z = aff.m[2] * v.x + aff.m[6] * v.y + aff.m[10]  * v.z;

}
//<<<<<<<<<<<<<<<<<<<<<<<<<< xfrmRay >>>>>>>>>>>>>>>>>>>>>
void xfrmRay(Ray& g, Affine4& aff, Ray& r)
{// g = aff * r // only the start part gets the translation component.
	g.start.x= aff.m[0] * r.start.x + aff.m[4] * r.start.y + aff.m[8]  * r.start.z + aff.m[12];
	g.start.y= aff.m[1] * r.start.x + aff.m[5] * r.start.y + aff.m[9]  * r.start.z + aff.m[13];
	g.start.z= aff.m[2] * r.start.x + aff.m[6] * r.start.y + aff.m[10] * r.start.z + aff.m[14];

	g.dir.x= aff.m[0] * r.dir.x + aff.m[4] * r.dir.y + aff.m[8]  * r.dir.z;
	g.dir.y= aff.m[1] * r.dir.x + aff.m[5] * r.dir.y + aff.m[9]  * r.dir.z;
	g.dir.z= aff.m[2] * r.dir.x + aff.m[6] * r.dir.y + aff.m[10]  * r.dir.z;
	
}
//<<<<<<<<<<<<<<<<<<<< xfrmNormal >>>>>>>>>>>...
void xfrmNormal(Vector3& res, Affine4& aff,Vector3& v)
{ // form res = transpose(aff) * v
	res.x = aff.m[0] * v.x + aff.m[1] * v.y + aff.m[2]  * v.z;
	res.y = aff.m[4] * v.x + aff.m[5] * v.y + aff.m[6]  * v.z;
	res.z = aff.m[8] * v.x + aff.m[9] * v.y + aff.m[10] * v.z;

}
// Vector3 methods
Vector3 Vector3 :: cross(Vector3 b) //return this cross b
{
	Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
	return c;
}
float Vector3 :: dot(Vector3 b) // return this dotted with b
{return x * b.x + y * b.y + z * b.z;}

void Vector3 :: normalize()//adjust this vector to unit length
{
	double sizeSq = x * x + y * y + z * z;
	if(sizeSq < 0.0000001)
	{
		cerr << "\nnormalize() sees vector (0,0,0)!!!";
		x = 1; // lousy cope - can you do better?
		return; // returns (1,0,0) - yuck!
	}
	float scaleFactor = 1.0/(float)sqrt(sizeSq);
	x *= scaleFactor; y *= scaleFactor; z *= scaleFactor;
}
// Color3 methods
void Color3 ::add(Color3& src, Color3& refl)
{ // add the product of source color and reflection coefficient
	red   += src.red   * refl.red;
	green += src.green * refl.green;
	blue  += src.blue  * refl.blue;
}
void Color3:: add(Color3& colr)
{ // add colr to this color
	blue=blue+colr.blue;
	green=green+colr.green;
	red=red+colr.red;
}
void Color3 :: build4tuple(float v[])
{// load 4-tuple with this color: v[3] = 1 for homogeneous
	v[0] = red; v[1] = green; v[2] = blue; v[3] = 1.0f;
}
//Affine4 methods
Affine4::Affine4(){ // make identity transform
	m[0] = m[5]  = m[10] = m[15] = 1.0;
	m[1] = m[2]  = m[3]  = m[4]  = 0.0;
	m[6] = m[7]  = m[8]  = m[9]  = 0.0;
	m[11]= m[12] = m[13] = m[14] = 0.0;
}
void Affine4 :: setIdentityMatrix(){ // make identity transform
	m[0] = m[5]  = m[10] = m[15] = 1.0;
	m[1] = m[2]  = m[3]  = m[4]  = 0.0;
	m[6] = m[7]  = m[8]  = m[9]  = 0.0;
	m[11]= m[12] = m[13] = m[14] = 0.0;
}
void Affine4 ::set(Affine4 a)// set this matrix to a
{
	for(int i = 0; i < 16; i++)
		m[i]=a.m[i];
}
//<<<<<<<<<<<<<< preMult >>>>>>>>>>>>
void Affine4 ::preMult(Affine4 n)
{// postmultiplies this with n
	float sum;
	Affine4 tmp;
	tmp.set(*this); // tmp copy
	// following mult's : this = tmp * n
	for(int c = 0; c < 4; c++)
		for(int r = 0; r <4 ; r++)
		{
			sum = 0;
			for(int k = 0; k < 4; k++)
				sum += n.m[4 * k + r]* tmp.m[4 * c + k];
			m[4 * c + r] = sum;
		}// end of for loops
}// end of preMult()
//<<<<<<<<<<<< postMult >>>>>>>>>>>
void Affine4 ::postMult(Affine4 n){// postmultiplies this with n
	float sum;
	Affine4 tmp;
	tmp.set(*this); // tmp copy
	for(int c = 0; c < 4; c++)// form this = tmp * n
		for(int r = 0; r <4 ; r++)
		{
			sum = 0;
			for(int k = 0; k < 4; k++)
				sum += tmp.m[4 * k + r]* n.m[4 * c + k];
			m[4 * c + r] = sum;
		}// end of for loops
}
// AffineStack methods
//<<<<<<<<<<<<<<<<<<<< dup >>>>>>>>>>>>>>>>>
void AffineStack :: dup()
{
	AffineNode* tmp = new AffineNode;
	tmp->affn = new Affine4(*(tos->affn));
	tmp->invAffn = new Affine4(*(tos->invAffn));
	tmp->next = tos;
	tos = tmp;
}
//<<<<<<<<<<<<<<<<< setIdentity >>>>>>>>>>>>>>>>>>>
void AffineStack :: setIdentity() // make top item the identity matrix
{
	assert(tos != NULL);
	tos->affn->setIdentityMatrix();
	tos->invAffn->setIdentityMatrix();
}
//<<<<<<<<<<<<<<<<<< popAndDrop >>>>>>>>>>>>>>>>>>
void AffineStack :: popAndDrop()
{
	if(tos == NULL) return; // do nothing
	AffineNode *tmp = tos;
	tos = tos->next;
	delete tmp; // should call destructor, which deletes trices
}
//<<<<<<<<<<<<<<<<<<<, releaseAffines >>>>>>>>>>>>>
void AffineStack :: releaseAffines()
{ // pop and drop all remaining items
	while(tos) popAndDrop();
}
//<<<<<<<<<<<<<<<<<<< rotate >>>>>>>>>>>>>>>
void AffineStack :: rotate(float angle, Vector3 u)
{
	Affine4 rm; // make identity matrix
	Affine4 invRm;
	u.normalize(); // make the rotation axis unit length
	float ang = angle * 3.14159265/ 180; // deg to
	float c = cos(ang), s = sin(ang);
	float mc = 1.0 - c;
	 	//fill the 3x3 upper left matrix
	rm.m[0] = c + mc * u.x * u.x;
	rm.m[1] = mc * u.x * u.y + s * u.z;
	rm.m[2] = mc * u.x * u.z - s * u.y;
	rm.m[4] = mc * u.y * u.x - s * u.z;
	rm.m[5] = c + mc * u.y * u.y;
	rm.m[6] = mc * u.y * u.z + s * u.x;
	rm.m[8] = mc * u.z * u.x + s * u.y;
	rm.m[9] = mc * u.z * u.y - s * u.x;
	rm.m[10] = c + mc * u.z * u.z;
	// same for inverse : just sign of s is changed
	invRm.m[0] = c + mc * u.x * u.x;
	invRm.m[1] = mc * u.x * u.y - s * u.z;
	invRm.m[2] = mc * u.x * u.z + s * u.y;
	invRm.m[4] = mc * u.y * u.x + s * u.z;
	invRm.m[5] = c + mc * u.y * u.y;
	invRm.m[6] = mc * u.y * u.z - s * u.x;
	invRm.m[8] = mc * u.z * u.x - s * u.y;
	invRm.m[9] = mc * u.z * u.y + s * u.x;
	invRm.m[10] = c + mc * u.z * u.z;
	tos->affn->postMult(rm);
	tos->invAffn->preMult(invRm);
}
//<<<<<<<<<<<<<<<<<<<<, scale >>>>>>>>>>>>>>>>>>>>>>
void AffineStack :: scale(float sx, float sy, float sz)
{ // post-multiply top item by scaling
#define sEps 0.00001
	Affine4 scl;// make an identity
	Affine4 invScl;
	scl.m[0]  = sx;
	scl.m[5]  = sy;
	scl.m[10] = sz;// adjust it to a scaling matrix
	if(fabs(sx) < sEps || fabs(sy) < sEps || fabs(sz) < sEps)
	{
		cerr << "degenerate scaling transformation!\n";
	}
	invScl.m[0]  = 1/sx; invScl.m[5]  = 1/sy; invScl.m[10] = 1/sz;
	tos->affn->postMult(scl); //
	tos->invAffn->preMult(invScl);
}
//<<<<<<<<<<<<<<<<<<<<< translate >>>>>>>>>>>>>>>>>>>
void AffineStack :: translate(Vector3 d)
{
	Affine4 tr; // make identity matrix
	Affine4 invTr;
	tr.m[12] = d.x; tr.m[13] = d.y;	tr.m[14] = d.z;
	invTr.m[12] = -d.x;	invTr.m[13] = -d.y; invTr.m[14] = -d.z;
	tos->affn->postMult(tr);
	tos->invAffn->preMult(invTr);
}

// Material methods
//<<<<<<<<<<<<<<<<<<<< setDefault >>>>>>>>>>>>>>>>>
void Material :: setDefault(){
	textureType = 0; // for none
	numParams = 0;
	reflectivity = transparency = 0.0;
	speedOfLight = specularExponent = 1.0;
	specularFraction = 0.0;
	surfaceRoughness = 1.0;
	ambient.set(0.1f,0.1f,0.1f);
	diffuse.set(0.8f,0.8f,0.8f);
	specular.set(0,0,0);
	emissive.set(0,0,0);
}
//<<<<<<<<<<<<<<<<<<<< set >>>>>>>>>>>>>>>>>>>
void Material :: set(Material& m)
{
	textureType = m.textureType;
	numParams = m.numParams;
	for(int i = 0; i < numParams; i++) params[i] = m.params[i];
	transparency = m.transparency;
	speedOfLight = m.speedOfLight;
	reflectivity = m.reflectivity;
	specularExponent = m.specularExponent;
	specularFraction = m.specularFraction;
	surfaceRoughness = m.surfaceRoughness;
	ambient.set(m.ambient);
	diffuse.set(m.diffuse);
	specular.set(m.specular);
	emissive.set(m.emissive);
}
//<<<<<<<<<<<<<<<<<<<<<<<< texture >>>>>>>>>>>>>>>>>>>>>>>>>>>
/*Color3 Material :: texture(Point3 &p)
{
	Color3 txtColor, plainColor;
	switch (textureType)
	{
	case 0: 
		txtColor.set(1,1,1);//nothing happnes
		break;
	case -1:{//checkerboard
		float S[3];
		for(int i=0;i<3;i++)//init S[3]; make sure nonzero
		{
			S[i]=(params[i]>0.0)? params[i]:0.25;
		}//S[i]=0.25 is default value
		// if there is no params, all S[i] are set to default value(0.25)
		// if for example 2 params only S(0) and S(1) are set new values,S(2) is still set
		// to 0.25,because the 3rd param is 0.0 by default.
		
		if(numParams>=6)
		{ 
			//check params,make sure nonezero and smaller than 1.0
			if(params[3]>1.0) params[3]=1.0;
			if(params[3]<0.0) params[3]=0.0;
			if(params[4]>1.0) params[4]=1.0;
			if(params[4]<0.0) params[4]=0.0;
			if(params[5]>1.0) params[5]=1.0;
			if(params[5]<0.0) params[5]=0.0;
			plainColor.set(params[3],params[4],params[5]);//use this color as default color
		}
		else
			plainColor.set(1.0,1.0,1.0);//use white as default color
	//make sure the ambient and diffuse reflection coefficient don't have zero component
		if(ambient.red==0.0) ambient.red=0.001;	
		if(ambient.green==0.0) ambient.green=0.001;
		if(ambient.blue==0.0) ambient.blue=0.001;

		if(diffuse.red==0.0) diffuse.red=0.001;
		if(diffuse.green==0.0) diffuse.green=0.001;
		if(diffuse.blue==0.0) diffuse.blue=0.001;
	//start the checkerboard jump function
		int jump;
	
		jump=((int)((p.x+1000)/S[0])+(int)((p.y+1000)/S[1])+(int)((p.z+1000)/S[2]))%2;//jump=0,1
		//adding 1000 to make sure nonneg values
		if(jump==1) txtColor.set(1,1,1);
			else txtColor.set(plainColor.red/diffuse.red,plainColor.green/diffuse.green,
			plainColor.blue/diffuse.blue);
		//there is assumption that diffuse and ambient have the same rgb ratios
		//the idea here is if jump==0 the color is set to the plainColor and 
		//if jump==1 the color is set to the diffuse color
			break;}
//	case -2://smooth change of color
//		break;
	case -3:{//wood 
		
		
		int rings;
		float M,K,N,B,D;//D is the offset of center from z-axis in x-axis
		switch(numParams)
		{
		case 0:{
			 M=.1;K=2;N=PI*2;B=0.3;D=2;//default values if not specified
			 break;}
		case 1:{
			 M=params[0];K=2;N=PI*2;B=0.3;D=2;
			 break;}
		case 2:{
			 M=params[0];K=params[1];N=PI*2;B=0.3;D=2;
			 break;}
		case 3:{
			 M=params[0];K=params[1];N=params[2];B=0.3;D=2;
			 break;}
		case 4:{
			 M=params[0];K=params[1];N=params[2];B=params[3];D=2;
			 break;}
		case 5:{
			M=params[0];K=params[1];N=params[2];B=params[3];D=params[4];
			 break;}
		default:{
			 M=.1;K=2;N=PI*2;B=0.3;D=2;;
			 break;}
		}
		
		float r=sqrt((p.x+D)*(p.x+D)+p.y*p.y);
		float theta;
		if(p.z!=0.0) theta=atan(r/p.z);
		else theta=PI/2;

		if(numParams>=8)
		{ 
			//check params,make sure nonezero and smaller than 1.0
			if(params[6]>1.0) params[6]=1.0;
			if(params[6]<0.0) params[6]=0.0;
			if(params[7]>1.0) params[7]=1.0;
			if(params[7]<0.0) params[7]=0.0;
			if(params[5]>1.0) params[5]=1.0;
			if(params[5]<0.0) params[5]=0.0;
			plainColor.set(params[5],params[6],params[7]);//use this color as default color
		}
		else
			plainColor.set(1.0,1.0,1.0);//use white as default color
    //make sure the ambient and diffuse reflection coefficient don't have zero component
		if(ambient.red==0.0) ambient.red=0.001;	
		if(ambient.green==0.0) ambient.green=0.001;
		if(ambient.blue==0.0) ambient.blue=0.001;

		if(diffuse.red==0.0) diffuse.red=0.001;
		if(diffuse.green==0.0) diffuse.green=0.001;
		if(diffuse.blue==0.0) diffuse.blue=0.001;
	//start the checkerboard jump function
		rings=r/M+K*sin(theta/M+B*p.z)+10000;
		rings=((int)rings)%2;
		if(rings==1) txtColor.set(1,1,1);
		else txtColor.set(plainColor.red/diffuse.red,plainColor.green/diffuse.green,
			plainColor.blue/diffuse.blue);
		break;}
	case -4:{//marble

		//Noise n;
		float marbleColor;
		marbleColor=n.marble(6,p);
		txtColor.set(marbleColor,marbleColor,marbleColor);
		break;}
	default:
		txtColor.set(1,1,1);
		break;
	}
	return txtColor;

}
// end of Material :: texture
*/
// Shape methods
//<<<<<<<<<<<<<<<<< tellMaterialsGL >>>>>>>>>>>>>>>>>>
void Shape :: tellMaterialsGL()
{
	float amb[4],diff[4],spec[4], emiss[4];
	float zero[] = {0,0,0,1};
	mtrl.ambient.build4tuple(amb); // fill the array
	mtrl.diffuse.build4tuple(diff);
	mtrl.specular.build4tuple(spec);
	mtrl.emissive.build4tuple(emiss);
	glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_AMBIENT,amb);
	glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_DIFFUSE,diff);
			glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_SPECULAR,spec);
			glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_EMISSION,emiss);
			glMaterialf(GL_FRONT/*_AND_BACK*/,GL_SHININESS,mtrl.specularExponent);
}
//Mesh methods
//<<<<<<<<<<<<<<<<< constructor >>>>>>>>>>>>>>>>>>>
Mesh :: Mesh(){
			numVerts = numFaces = numNorms = 0;
			pt = NULL; norm  =  NULL; face = NULL;
			lastVertUsed = lastNormUsed = lastFaceUsed = -1;
}
//<<<<<<<<<<<<<<<<<<<<<< freeMesh >>>>>>>>>>>>>>>>>>
void Mesh :: freeMesh()
{ // free up memory used by this mesh.
			delete [] pt; // release whole vertex list
			delete [] norm;
			for(int f = 0; f < numFaces; f++)
				delete[] face[f].vert; // delete the vert[] array of this face
			delete [] face;
}
//<<<<<<<<<<<<<<<<<<<< isEmpty >>>>>>>>>>>>>>>>>>>>
int Mesh :: isEmpty()
{
	return (numVerts == 0) || (numFaces == 0) || (numNorms == 0);
}
//<<<<<<<<<<<<<<<<<<<<< makeEmpty >>>>>>>>>>>>>>>>>>>>>.
void Mesh :: makeEmpty()
{
	numVerts = numFaces = numNorms = 0;
}
//<<<<<<<<<<<<<<<<<<< drawOpenGL >>>>>>>>>>>>>>>>>>>
void Mesh :: drawOpenGL()
{
			tellMaterialsGL(); 	glPushMatrix();
			glMultMatrixf(transf.m);
			drawMesh();
			glPopMatrix();
}
//<<<<<<<<<<<<<<<<<< constructor >>>>>>>>>>>>>>>>
Mesh :: Mesh(string fname){ // read this file to build mesh
			numVerts = numFaces = numNorms = 0;
			pt = NULL; norm  =  NULL; face = NULL;
			lastVertUsed = lastNormUsed = lastFaceUsed = -1;
			readMesh(fname);
}
//<<<<<<<<<<<<<<<<<<< newell4 >>>>>>>>>>>>>>>>>>>>
Vector3 Mesh :: newell4(int indx[])
{ /* return the normalized normal to face with vertices
  pt[indx[0]],...,pt[indx[3]]. i.e. indx[] contains the four indices
	into the vertex list to be used in the Newell calculation */
			Vector3 m;
			for(int i = 0; i < 4 ; i++)
			{
				int next = (i== 3) ? 0 : i + 1; // which index is next?
				int f = indx[i], n = indx[next]; // names for the indices in the pair
				m.x += (pt[f].y - pt[n].y) * (pt[f].z + pt[n].z);
				m.y += (pt[f].z - pt[n].z) * (pt[f].x + pt[n].x);
				m.z += (pt[f].x - pt[n].x) * (pt[f].y + pt[n].y);
			}
			m.normalize();
			return m;
}
//<<<<<<<<<<<<<<<<<<<<< readMesh >>>>>>>>>>>>>>>>>>>>>>>>
void Mesh:: readMesh(string fname)
{
	fstream inStream;
	inStream.open(fname.c_str(), ios ::in); //open needs a c-like string
	if(inStream.fail() || inStream.eof())
	{
		cout << "can't open file or eof: " << fname << endl;
		makeEmpty();return;
	}
	inStream >> numVerts >> numNorms >> numFaces;
	// make arrays for vertices, normals, and faces
	pt = new Point3[numVerts];        assert(pt != NULL);
	norm = new Vector3[numNorms];     assert(norm != NULL);
	face = new Face[numFaces];        assert(face != NULL);
	for(int i = 0; i < numVerts; i++) 	// read in the vertices
		inStream >> pt[i].x >> pt[i].y >> pt[i].z;
	for(int ii = 0; ii < numNorms; ii++) 	// read in the normals
		inStream >> norm[ii].x >> norm[ii].y >> norm[ii].z;
	for(int f = 0; f < numFaces; f++)   // read in face data
	{
		inStream >> face[f].nVerts;
		int n = face[f].nVerts;
		face[f].vert = new VertexID[n]; assert(face[f].vert != NULL);
		for(int k = 0; k < n; k++) 		// read vertex indices for this face
			inStream >> face[f].vert[k].vertIndex;
		for(int kk = 0; kk < n; kk++) 		// read normal indices for this face
			inStream >> face[f].vert[kk].normIndex;
	}
	inStream.close();
} // end of readMesh
//<<<<<<<<<<<<<<<<<<<<<< drawMesh >>>>>>>>>>>>>>>>>>>>
void Mesh :: drawMesh()
{ // draw each face of this mesh using OpenGL: draw each polygon.
	if(isEmpty()) return; // mesh is empty
	for(int f = 0; f < numFaces; f++)
	{
		int n = face[f].nVerts;
		glBegin(GL_POLYGON);
		for(int v = 0; v < n; v++)
		{
			int in = face[f].vert[v].normIndex;
			assert(in >= 0 && in < numNorms);
			glNormal3f(norm[in].x, norm[in].y, norm[in].z);
			int iv = face[f].vert[v].vertIndex; assert(iv >= 0 && iv < numVerts);
			glVertex3f(pt[iv].x, pt[iv].y, pt[iv].z);
		}
		glEnd();
	}
	glFlush();
}
//<<<<<<<<<<<<<<<<<< hit() for Sphere >>>>>>>>>>>>>>>>>>>>>>>
bool Sphere:: hit(Ray &r, Intersection& inter)
{  // intersect the ray with the generic sphere
		Ray genRay;
		xfrmRay(genRay,invTransf,r);
	
		double A, B, C, discrim, disc_root, t1, t2;
		A = dot3D(genRay.dir, genRay.dir); assert(A > 0); // fails if ray is sick
		B = dot3D(genRay.start, genRay.dir);
		C = dot3D(genRay.start, genRay.start) - 1.0;
		discrim = B * B - A * C;
		if(discrim < 0.0)
		{
			return false;   // ray missed: numHits = 0
			cout<<"miss ball";
		}
		disc_root = sqrt(discrim);

		inter.numHits = 0;	// no hits yet//don't need consider other obj, done in Scen.shade
		t1 = (-B - disc_root)/A;         // earlier hit
		if(t1 > 0.00001) // a pos hit time
		{
			inter.hit[inter.numHits].hitTime = t1;
			Point3 P(rayPos(genRay, t1));// position of hit spot
			inter.hit[inter.numHits].hitPoint.set(P);
			inter.hit[inter.numHits].hitObject = this;
			inter.hit[inter.numHits].isEntering = 1;
			inter.hit[inter.numHits].surface = 0;
			inter.hit[inter.numHits].hitNormal.set(P.x, P.y, P.z); // vector is same as hit spot
			inter.numHits++; // got one hit so far
		}
		t2 = (-B + disc_root)/A; 		// second hit
		if(t2 > 0.00001) // got a hit (first or second...)
		{
			inter.hit[inter.numHits].hitTime = t2;
			Point3 P(rayPos(genRay, t2));  // position of hit spot
			inter.hit[inter.numHits].hitPoint.set(P);
			inter.hit[inter.numHits].isEntering = 0;
			inter.hit[inter.numHits].hitObject = this;
			inter.hit[inter.numHits].surface = 0;
			inter.hit[inter.numHits].hitNormal.set(P.x, P.y, P.z); // vector is same as hit spot
			inter.numHits++;			// got another
		}
		return inter.numHits > 0;
}// end of hit() for Sphere
bool TaperedCylinder:: hit(Ray &r, Intersection& inter)
{  // intersect the ray with the TaperedCylinder
		Ray genRay;
		xfrmRay(genRay,invTransf,r);
	
		double A, B, C, discrim, disc_root, t1, t2,tb,tc;
		double sm=smallRadius-1;
		double fDir=sm*genRay.dir.z;
		double fStart=sm*genRay.start.z+1;
		
		
		//A = dot3D(genRay.dir, genRay.dir)-genRay.dir.z*genRay.dir.z-fDir*fDir; 
		//B = dot3D(genRay.start, genRay.dir)-genRay.dir.z*genRay.start.z-fDir*fStart;
		//C = dot3D(genRay.start, genRay.start)-genRay.start.z*genRay.start.z - fStart*fStart;
		A=pow(genRay.dir.x,2)+pow(genRay.dir.y,2)-pow(fDir,2);		
		B=genRay.dir.x*genRay.start.x+genRay.dir.y*genRay.start.y-fDir*fStart;		
		C=pow(genRay.start.x,2)+pow(genRay.start.y,2)-pow(fStart,2);
		discrim = B * B - A * C;
		
		int num=0;

		if(discrim > 0.0)
		{
			disc_root = (double)sqrt(double(discrim));
			
			t1 = (-B - disc_root)/A;         // earlier hit 
			float zHit=genRay.start.z+genRay.dir.z*t1;//z component of ray
			//zHit between 0 and 1 makes sure hit between two caps
			if(t1 > 0.00001 && zHit<=1.0 && zHit>=0) // a pos hit time
			{
				inter.hit[num].hitTime = t1;
				inter.hit[num++].  surface = 0;
				
			}
			
			t2 = (-B + disc_root)/A; 		// second hit 
			zHit=genRay.start.z+genRay.dir.z*t2;
			if(t2 > 0.00001 && zHit<=1.0 && zHit>=0) // got a hit (first or second...)
			{
				inter.hit[num].hitTime = t2;				
				inter.hit[num++].surface = 0;				
				
			}
		}// end of if(discrim>0)

		tb=-genRay.start.z/genRay.dir.z;//hit time at z=0 plane
		if(tb>0.00001 && pow(genRay.start.x+genRay.dir.x*tb,2)+
			pow(genRay.start.y+genRay.dir.y*tb,2)<1.0 )//within disc of base
		{
			inter.hit[num].hitTime=tb;
			inter.hit[num++].surface=1;//1 for the base
			
		}
		//test the cap at z=1
		tc=(1-genRay.start.z)/genRay.dir.z;//hit time at z=1 plane
		if(tc>0.00001 && pow(genRay.start.x+genRay.dir.x*tc,2)+
			pow(genRay.start.y+genRay.dir.y*tc,2)<pow(smallRadius,2))//within disc of top
		{
			inter.hit[num].hitTime=tc;
			inter.hit[num++].surface=2;//2 for the cape
			
		}
		if (num==0) return false;			
		
		inter.numHits=num;
		if (num==1)//eye inside the cylinder
		{
			inter.hit[0].isEntering=false;
			inter.hit[0].hitObject=this;
		}
		else//have two hits-first must be entering
		{
			if(inter.hit[0].hitTime>inter.hit[1].hitTime)
			{
				double tmpT=inter.hit[0].hitTime;
				inter.hit[0].hitTime=inter.hit[1].hitTime;
				inter.hit[1].hitTime=tmpT;

				int tmpS=inter.hit[0].surface;
				inter.hit[0].surface=inter.hit[1].surface;
				inter.hit[1].surface=tmpS;
			}
			inter.hit[0].isEntering=true;
			inter.hit[1].isEntering=false;
			inter.hit[0].hitObject=this;
			inter.hit[1].hitObject=this;
		}//end of if(num==1)...else
		Point3 P0;
		for (int i=0;i<num;i++)
		{
			P0.set(rayPos(genRay,inter.hit[i].hitTime));
			inter.hit[i].hitPoint.set(P0);
			int surf=inter.hit[i].surface;
			if(surf==0)
				inter.hit[i].hitNormal.set(P0.x,P0.y,-sm*(1+sm*P0.z));
			else if(surf==1)
				inter.hit[i].hitNormal.set(0,0,-1);
			else inter.hit[i].hitNormal.set(0,0,1);
		}
		
		return true;
}// end of hit() for TaperedCylinder
bool TaperedTube:: hit(Ray &r, Intersection& inter)
{  // intersect the ray with the TaperedTube
		Ray genRay;
		xfrmRay(genRay,invTransf,r);
	
		double A, B, C, discrim, disc_root, t1, t2,tb,tc;
		double sm=smallRadius-1;
		double fDir=sm*genRay.dir.z;
		double fStart=sm*genRay.start.z+1;
	
		
		//for the outer shell
		A=pow(genRay.dir.x,2)+pow(genRay.dir.y,2)-pow(fDir,2);		
		B=genRay.dir.x*genRay.start.x+genRay.dir.y*genRay.start.y-fDir*fStart;		
		C=pow(genRay.start.x,2)+pow(genRay.start.y,2)-pow(fStart,2);
		discrim = B * B - A * C;
		
		int num=0;
		
		if(discrim > 0.0)
		{
			disc_root = (double)sqrt(double(discrim));
			
			t1 = (-B - disc_root)/A;         // earlier hit of outer shell
			float zHit=genRay.start.z+genRay.dir.z*t1;//z component of ray
			//zHit between 0 and 1 makes sure hit between two caps
			if(t1 > 0.00001 && zHit<=1.0 && zHit>=0) // a pos hit time
			{
				inter.hit[num].hitTime = t1;
				inter.hit[num++].  surface = 0;
				
			}
			
			t2 = (-B + disc_root)/A; 		// second hit of outer shell
			zHit=genRay.start.z+genRay.dir.z*t2;
			if(t2 > 0.00001 && zHit<=1.0 && zHit>=0) // got a hit (first or second...)
			{
				inter.hit[num].hitTime = t2;				
				inter.hit[num++].surface = 0;				
				
			}
		}// end of outer shell if(discrim>0)

		//for inner shell
		
		fStart=sm*genRay.start.z+1-thickness;	
		B=genRay.dir.x*genRay.start.x+genRay.dir.y*genRay.start.y-fDir*fStart;		
		C=pow(genRay.start.x,2)+pow(genRay.start.y,2)-pow(fStart,2);
		discrim = B * B - A * C;
		if(discrim > 0.0)
		{
			disc_root = (double)sqrt(double(discrim));
			
			t1 = (-B - disc_root)/A;         // earlier hit of inner shell
			float zHit=genRay.start.z+genRay.dir.z*t1;//z component of ray
			//zHit between 0 and 1 makes sure hit between two caps
			if(t1 > 0.00001 && zHit<=1.0 && zHit>=0) // a pos hit time
			{
				inter.hit[num].hitTime = t1;
				inter.hit[num++].  surface = 0;
				
			}
			
			t2 = (-B + disc_root)/A; 		// second hit of inner shell
			zHit=genRay.start.z+genRay.dir.z*t2;
			if(t2 > 0.00001 && zHit<=1.0 && zHit>=0) // got a hit (first or second...)
			{
				inter.hit[num].hitTime = t2;				
				inter.hit[num++].surface = 0;				
				
			}
		}// end of inner shell if(discrim>0)

		tb=-genRay.start.z/genRay.dir.z;//hit time at z=0 plane
		if(tb>0.00001 && pow(genRay.start.x+genRay.dir.x*tb,2)+
			pow(genRay.start.y+genRay.dir.y*tb,2)<1.0 && pow(genRay.start.x+genRay.dir.x*tb,2)+
			pow(genRay.start.y+genRay.dir.y*tb,2)>pow(1.0-thickness,2))//within disc of base
		{
			inter.hit[num].hitTime=tb;
			inter.hit[num++].surface=1;//1 for the base			
		}
		//test the cap at z=1
		tc=(1-genRay.start.z)/genRay.dir.z;//hit time at z=1 plane
		if(tc>0.00001 && pow(genRay.start.x+genRay.dir.x*tc,2)+
			pow(genRay.start.y+genRay.dir.y*tc,2)<pow(smallRadius,2) &&
			pow(genRay.start.x+genRay.dir.x*tc,2)+pow(genRay.start.y+genRay.dir.y*tc,2)
			>pow(smallRadius-thickness,2))//within disc of top
		{
			inter.hit[num].hitTime=tc;
			inter.hit[num++].surface=2;//2 for the cape
		}
		if (num==0) return false;			
		
		inter.numHits=num;
			
		Point3 P0;
		double tmpT;
		int tmpS;
		for (int i=0;i<num-1;i++)
		{
			
			for (int j=0;j<num-1-i;j++)//bubbling method to sort hit[] by hitTime
			{
				if(inter.hit[j].hitTime>inter.hit[j+1].hitTime)
				{
					tmpT=inter.hit[j].hitTime;
					inter.hit[j].hitTime=inter.hit[j+1].hitTime;
					inter.hit[j+1].hitTime=tmpT;

					tmpS=inter.hit[j].surface;
					inter.hit[j].surface=inter.hit[j+1].surface;
					inter.hit[j+1].surface=tmpS;
				}

			}
				
			inter.hit[i].hitObject=this;
			
			P0.set(rayPos(genRay,inter.hit[i].hitTime));
			inter.hit[i].hitPoint.set(P0);
			int surf=inter.hit[i].surface;
			if(surf==0)
				inter.hit[i].hitNormal.set(P0.x,P0.y,-sm*(1+sm*P0.z));
			else if(surf==1)
				inter.hit[i].hitNormal.set(0,0,-1);
			else if(surf==2)inter.hit[i].hitNormal.set(0,0,1);
			else inter.hit[i].hitNormal.set(-P0.x,-P0.y,+sm*(1+sm*P0.z));//normal of inner shell
			
		}

		if (num==1)//eye between the two shells and looking outside
		{
			inter.hit[0].isEntering=false;
		}
		else if (num==2)//have two hits-first must be entering
		{			
			inter.hit[0].isEntering=true;
			inter.hit[1].isEntering=false;

		}
		else if (num==4)//have 4 hits-first must be entering
		{
			inter.hit[0].isEntering=true;
			inter.hit[1].isEntering=false;
			inter.hit[2].isEntering=true;
			inter.hit[3].isEntering=false;
		}
		else if (num==3)// eye must be between 2 shells and looking at the inner shell
		{
			inter.hit[0].isEntering=false;
			inter.hit[1].isEntering=true;
			inter.hit[2].isEntering=false;
		}		
		return true;
		

}// end of hit() for TaperedTube
bool Square:: hit(Ray &r, Intersection& inter)
{  // intersect the ray with the generic sphere
		Ray genRay;
		xfrmRay(genRay,invTransf,r);

		inter.numHits=0;
		double denom = genRay.dir.z;
		if(fabs(denom)<0.00001) return false;
		double time=-genRay.start.z/denom;//hit time
		if(time<=0.0) return false;
		double hx=genRay.start.x+genRay.dir.x*time;
		double hy=genRay.start.y+genRay.dir.y*time;
		if((hx>1.0)||(hx<-1.0)) return false;
		if((hy>1.0)||(hy<-1.0)) return false;
		inter.numHits=1;
		inter.hit[0].hitObject=this;
		inter.hit[0].hitTime=time;
		inter.hit[0].isEntering=true;
		inter.hit[0].surface=0;
		inter.hit[0].hitNormal.set(0,0,1);
		return true;	
		
}// end of hit() for Square
Vector3 Cube::cubeNormal(int surface)
{
	Vector3 cubeN;
	
	switch(surface)
	{
	case 0: cubeN.set(0,1,0);break;
	case 1: cubeN.set(0,-1,0);break;
	case 2: cubeN.set(1,0,0);break;
	case 3: cubeN.set(-1,0,0);break;
	case 4: cubeN.set(0,0,1);break;
	case 5: cubeN.set(0,0,-1);break;
	}
	return cubeN;
}

bool Cube:: hit(Ray &r, Intersection& inter)
{  // intersect the ray with the generic sphere
		Ray genRay;
		xfrmRay(genRay,invTransf,r);
	    
		double tHit,numer,denom;
		double tIn=-100000.0, tOut=100000.0;
		int inSurf,outSurf;
		for(int i=0; i<6; i++)
		{
			switch(i)
			{
			case 0: numer=1.0-genRay.start.y;
				    denom=genRay.dir.y;
					break;
			case 1: numer=1.0+genRay.start.y;
				    denom=-genRay.dir.y;
					break;
			case 2: numer=1.0-genRay.start.x;
				    denom=genRay.dir.x;
					break;
			case 3: numer=1.0+genRay.start.x;
				    denom=-genRay.dir.x;
					break;
			case 4: numer=1.0-genRay.start.z;
				    denom=genRay.dir.z;
					break;
			case 5: numer=1.0+genRay.start.z;
				    denom=-genRay.dir.z;
					break;
			}
			if(fabs(denom)<0.00001)
				if(numer<0) return false;
				else;//ray inside, no change to tIn, tOut
			else//ray is not parallel
			{
				tHit=numer/denom;
				if(denom>0)//exiting
				{
					if(tHit<tOut)//a new earlier exit
					{
						tOut=tHit;
						outSurf=i;
					}
				}
				else //denom is negative: entering
				{
					if(tHit>tIn)//a new later entrance
					{
						tIn=tHit;
						inSurf=i;
					}
				}
			}
			if (tIn>=tOut) return false;// it's a miss- early out
		}//end of the for loop
		int num=0;//no positive hits yet
		if(tIn>0.00001)// is first hit in front of the eye
		{
			inter.hit[0].hitTime=tIn;
			inter.hit[0].surface=inSurf;
			inter.hit[0].isEntering=1;//is entering
			inter.hit[0].hitObject=this;
			inter.hit[0].hitPoint.set(rayPos(genRay.start,genRay.dir,tIn));
			inter.hit[0].hitNormal.set(cubeNormal(inSurf));
			num++;// have a hit
		}
		if(tOut>0.00001)
		{
			inter.hit[num].hitTime=tOut;
			inter.hit[num].surface=outSurf;
			inter.hit[num].isEntering=0;
			inter.hit[num].hitObject= this;
			inter.hit[num].hitPoint.set(rayPos(genRay.start,genRay.dir,tOut));
			inter.hit[num].hitNormal.set(cubeNormal(outSurf));
			num++;
		}
		inter.numHits=num;
		return (num>0);

}// end of hit() for Cube
bool Mesh::hit(Ray &r, Intersection &inter)
{
	Ray genRay;
	xfrmRay(genRay, invTransf, r);

	double tHit, numer, denom;
	double tIn = -100000.0,tOut = 100000.0;
	
	int inSurf, outSurf;//entering and exiting surface
	Point3 Vert0;
	Vector3 diff, normal;

	
	for (int k = 0; k < numFaces; k++)//search hitpoint at kth surface
	{
		normal.set(norm[face[k].vert[0].normIndex]);
		Vert0.set(pt[face[k].vert[0].vertIndex]);
		
		diff.setDiff(Vert0, genRay.start);
		numer = dot3D(normal, diff);
		denom = dot3D(normal, genRay.dir);

		if(fabs(denom) < 0.00001) // ray is parallel
			if(numer < 0) 
				return false; //ray is out;
			else ; // ray inside, no change to tIn,tOut
		else // ray is not parallel
		{
			tHit = numer / denom;
			if(denom > 0){ // exiting
				if(tHit < tOut) // a new earlier exit
				{ 
					tOut = tHit; 
					outSurf = k;
				}
			}
			else { // denom is negative: entering
				if(tHit > tIn) // a new later entrance
				{ 
					tIn = tHit; 
					inSurf = k;
				}
			}
		}
		if(tIn >= tOut)
			return false; // it's a miss - early out
	} // end of the for loop
	int num = 0; // no positive hits yet
	if(tIn > 0.00001) //is first hit in front of the eye?
	{
		inter.hit[num].hitTime = tIn;
		inter.hit[num].surface = inSurf;
		inter.hit[num].isEntering = 1; // is entering
		inter.hit[num].hitObject = this;
		inter.hit[num].hitPoint.set(rayPos(genRay.start, genRay.dir,tIn));
		inter.hit[num].hitNormal.set(norm[face[inSurf].vert[0].normIndex]);
		num++; //got a hit
	}
	if(tOut > 0.00001)
	{
		inter.hit[num].hitTime = tOut;
		inter.hit[num].surface = outSurf;
		inter.hit[num].isEntering = 0; // is exiting
		inter.hit[num].hitObject = this;
		inter.hit[num].hitPoint.set(rayPos(genRay.start,genRay.dir,tOut));
		inter.hit[num].hitNormal.set(norm[face[outSurf].vert[0].normIndex]);
		num++;
	}
	inter.numHits = num; //number of hits in front of eye
	return (num > 0);
};
//end of Mess:: hit
// Scene methods
//<<<<<<<<<<<<<<<<< nextToken>>>>>>>>>>>>>>>>>>>
string Scene :: nexttoken(void)
{
	char c;
	string token;
	int lastchar = 1;
	if (!f_in) {return(token); }
	if (f_in->eof()) {return(token);}
	while (f_in->get(c))
	{
		if (f_in->eof()) {
			return(token);
		}
		switch (c) {
		case '\n': nextline += 1;
		case ' ' :
		case '\t':
		case '\a':
		case '\b':
		case '\v':
		case '\f':
		case '\r': {
			if ( lastchar == 0 ) {return(token);}break; }
		case '{': {
			token = c; return(token); break;}
		case '}': {
			token = c;
			return(token);
			break; }
		case '!': {
			while ( c != '\n' && f_in->get(c)) {
			}
			nextline++; break;}
		default: {
			token = token + c;
			lastchar = 0;

			if ((f_in->peek() == '{') ||
				(f_in->peek() == '}') ) {
				if ( lastchar == 0 ) {
					return(token);
				} else {
					f_in->get(c);
					token = c;
					return(token);
				}
			}
			line = nextline;
					}
		}
	}
	return(" ");
}
//<<<<<<<<<<<<<< getFloat >>>>>>>>>>>>>>>>
float Scene :: getFloat() //############ getFloat()
{
	strstream tmp;
	float number;
	string str = nexttoken();
	tmp << str;
	if(!(tmp >> number))
	{
		cerr << "Line " << line << ": error getting float" << endl;
		exit(-1);
	}
	else
	{
		char t;
		if ( (tmp >> t ) )
		{
			cerr << "Line " << line << ": bum chars in number" << endl;
			exit(-1);
		}
	}
	return number;
}
//<<<<<<<<<<<<<<<<< isidentifier >>>>>>>>>>>>>>>>
bool Scene :: isidentifier(string keyword) { //######## isidentifier
	string temp = keyword;
	if (!isalpha(temp[0])) return(false);
	for (int count = 1; count < temp.length(); count++) {
		if ((!isalnum(temp[count]))&& (temp[count]!='.')) return(false);
	}
	return(true);
}
//<<<<<<<<<<<<<<<< cleanUp >>>>>>>>>>>>>>>>
void Scene :: cleanUp() //######### cleanUp
{ // release stuff after parsing file
	affStk.releaseAffines(); 		//delete affine stack
	def_stack->release();
	delete def_stack; // release the DefUnitStack memory
}
//<<<<<<<<<<<<<<<<< freeScene >>>>>>>>>>>>>>
void Scene :: freeScene()
{ // release the object and light lists
	GeomObj *p = obj;
	while(p)
	{
		GeomObj* q = p;
		p = p->next;
		delete q;
	}
	Light * q = light;
	while(q)
	{
		Light* r = q;
		q = q->next;
		delete r;
	}
}
//<<<<<<<<<<<<<<<<< whichToken >>>>>>>>>>>>>>>
mTokenType Scene :: whichtoken(string keyword)
{
	string temp = keyword;
	if ( temp == "light" )			  return LIGHT;
	if ( temp == "rotate" )           return ROTATE;
	if ( temp == "translate" )        return TRANSLATE;
	if ( temp == "scale")             return (SCALE);
	if ( temp == "push")        	  return (PUSH);
	if ( temp == "pop")				  return (POP);
	if ( temp == "identityAffine")    return (IDENTITYAFFINE);
	if ( temp == "cube")              return (CUBE);
	if ( temp == "sphere")            return (SPHERE);
	if ( temp == "torus")             return (TORUS);
	if ( temp == "plane")             return (PLANE);
	if ( temp == "square")            return (SQUARE);
	if ( temp == "cylinder")          return (CYLINDER);
	if ( temp == "taperedCylinder")   return (TAPEREDCYLINDER);
	if ( temp == "taperedTube")       return (TAPEREDTUBE);
	if ( temp == "cone")              return (CONE);
	if ( temp == "tetrahedron")       return (TETRAHEDRON);
	if ( temp == "octahedron")        return (OCTAHEDRON);
	if ( temp == "dodecahedron")      return (DODECAHEDRON);
	if ( temp == "icosahedron")       return (ICOSAHEDRON);
	if ( temp == "buckyball")         return (BUCKYBALL);
	if ( temp == "diamond")           return (DIAMOND);
	if ( temp == "teapot")			  return (TEAPOT);
	if ( temp == "union")             return (UNION);
	if ( temp == "intersection")      return (INTERSECTION);
	if ( temp == "difference")        return (DIFFERENCEa);
	if ( temp == "mesh")              return (MESH);
	if ( temp == "makePixmap")        return (MAKEPIXMAP);
	if ( temp == "defaultMaterials")  return (DEFAULTMATERIALS);
	if ( temp == "ambient")           return (AMBIENT);
	if ( temp == "diffuse")           return (DIFFUSE);
	if ( temp == "specular")          return (SPECULAR);
	if ( temp == "specularFraction")  return (SPECULARFRACTION);
	if ( temp == "surfaceRoughness")  return (SURFACEROUGHNESS);
	if ( temp == "emissive")          return (EMISSIVE);
	if ( temp == "specularExponent")  return (SPECULAREXPONENT);
	if ( temp == "speedOfLight")      return (SPEEDOFLIGHT);
	if ( temp == "transparency")      return (TRANSPARENCY);
	if ( temp == "reflectivity")      return (REFLECTIVITY);
	if ( temp == "parameters")        return (PARAMETERS);
	if ( temp == "texture")			  return (TEXTURE);
	if ( temp == "globalAmbient")	  return (GLOBALAMBIENT);
	if ( temp == "minReflectivity")	  return (MINREFLECTIVITY);
	if ( temp == "minTransparency")	  return (MINTRANSPARENCY);
	if ( temp == "maxRecursionDepth") return (MAXRECURSIONDEPTH);
	if ( temp == "background")        return (BACKGROUND);
	if ( temp == "{")                 return (LFTCURLY);
	if ( temp == "}")                 return (RGHTCURLY);
	if ( temp == "def")               return (DEF);
	if ( temp == "use")               return (USE);
	if ( temp == " " )                return (T_NULL);
	if ( isidentifier(temp) )         return (IDENT);
	cout << temp << ":" << temp.length() << endl;
	return(UNKNOWN);
} // end of whichtoken

//<<<<<<<<<<<<<<<<<<<<<  drawSceneOpenGL >>>>>>>>>>>>>>>>>>>>>>>.
void Scene :: drawSceneOpenGL()
{ //draw each object on object list
			for(GeomObj* p = obj; p ; p = p->next)
				p->drawOpenGL(); //draw it
}
//<<<<<<<<<<<<<<< Scene :: read >>>>>>>>>>>>>>>>
bool Scene:: read(string fname)// return true if ok; else false
{
	file_in = new ifstream(fname.c_str());
	if(!(*file_in))
	{
		cout << "I can't find or open file: " << fname << endl;
		return false;
	}
	f_in = new strstream();
	line = nextline = 1;
	def_stack = new DefUnitStack();
	char ch;
	freeScene(); //delete any previous scene
	// initialize all for reading:
	obj = tail = NULL;
	light = NULL;
	affStk.tos = new AffineNode;
	affStk.tos->next = NULL;

	while (file_in->get(ch)) {*f_in << ch;} // read whole file
	while(1) //read file, collecting objects, until EOF or an error
	{
		GeomObj * shp = getObject(); // get the next shape
		if(!shp) break; // no object: either error or EOF
		shp->next = NULL; // to be safe
		if(obj == NULL){ obj = tail = shp;} // empty list so far
		else{tail->next = shp; tail = shp;} // add new object to queue
	}
	file_in->close();
	cleanUp(); // delete temp lists, etc.
	return true;
} // end of read()

//<<<<<<<<<<<<<< Scene :: getObject >>>>>>>>>>>>>>>
GeomObj* Scene :: getObject()
{ //reads tokens from stream f_in (a data member of Scene),
	// building lights, getting materials, doing transformations,
	// until it finds a new object
	// returns NULL if any error occurs, or end of file
	string s;
	GeomObj * newShape;
	mTokenType typ;
	while ((typ = (whichtoken( s = nexttoken() ))) != T_NULL)
	{
		if(typ == UNION || typ == INTERSECTION || typ == DIFFERENCEa)
		{
			switch(typ)
			{
			case UNION:				newShape = new UnionBool();	break;
			case INTERSECTION:	newShape = new IntersectionBool();	break;
			case DIFFERENCEa:		newShape = new DifferenceBool();break;
			} // end of little switch
			GeomObj* p = newShape;
			p = getObject(); // get left child
			if(!p) return NULL; // Error! should always get an object
			((Boolean*)newShape)->left  = p; // hook it up
			p = getObject();// get right child
			if(!p) return NULL;
			((Boolean*)newShape)->right = p; // hook it up
			return newShape;
		}// end of if(typ == UNION etc....
		switch(typ)
		{
		case LIGHT: {
			Point3 p;
			Color3 c;
			p.x = getFloat(); p.y = getFloat();	p.z = getFloat();
			c.red = getFloat(); c.green = getFloat();	c.blue = getFloat();
			Light *l = new Light;
			l->setPosition(p);
			l->setColor(c);
			l->next = light; //put it on the list
			light = l; break;}
		case ROTATE: {
			float angle;
			Vector3 u;
			angle = getFloat(); u.x = getFloat();
			u.y = getFloat(); u.z = getFloat();
			affStk.rotate(angle,u);break;}
		case TRANSLATE: {
			Vector3 d;
			d.x = getFloat(); d.y = getFloat(); d.z = getFloat();
			affStk.translate(d);break;}
		case SCALE: {
			float sx, sy, sz;
			sx = getFloat(); sy = getFloat(); sz = getFloat();
			affStk.scale(sx, sy, sz);break;}
		case PUSH: affStk.dup(); break;
		case POP:  affStk.popAndDrop(); break;
		case IDENTITYAFFINE: affStk.setIdentity();break;
		case AMBIENT: {
			float dr, dg, db;
			dr = getFloat(); dg = getFloat(); db = getFloat();
			currMtrl.ambient.set(dr,dg,db); break;}
		case DIFFUSE: {
			float dr,dg,db;
			dr = getFloat(); dg = getFloat(); db = getFloat();
			currMtrl.diffuse.set(dr,dg,db); break;}
		case SPECULAR:{
			float dr,dg,db;
			dr = getFloat(); dg = getFloat(); db = getFloat();
			currMtrl.specular.set(dr,dg,db); break;}
		case EMISSIVE: {
			float dr,dg,db;
			dr = getFloat(); dg = getFloat(); db = getFloat();
			currMtrl.emissive.set(dr,dg,db); break;}
		case PARAMETERS: { // get a list of numParams parameters
			currMtrl.numParams = (int)getFloat();
			for(int i = 0; i < currMtrl.numParams; i++)
				currMtrl.params[i] = getFloat();
			break;}
		case SPECULARFRACTION: currMtrl.specularFraction = getFloat(); break;
		case SURFACEROUGHNESS: currMtrl.surfaceRoughness = getFloat(); break;
		case TEXTURE: { // get type, 0 for none
			currMtrl.textureType = getFloat();
			//if(currMtrl.textureType=-4)
				//currMtrl.n= new Noise();
					  
					  }
			break;
		case DEFAULTMATERIALS: 	currMtrl.setDefault();break;
		case SPEEDOFLIGHT: currMtrl.speedOfLight = getFloat(); break;
		case SPECULAREXPONENT: currMtrl.specularExponent = getFloat(); break;
		case TRANSPARENCY:currMtrl.transparency = getFloat(); break;
		case REFLECTIVITY: currMtrl.reflectivity = getFloat(); break;
		case GLOBALAMBIENT:
			ambient.red = getFloat(); ambient.green = getFloat();
			ambient.blue = getFloat(); break;
		case BACKGROUND:
			background.red = getFloat();
			background.green = getFloat();
			background.blue = getFloat();break;
		case MINREFLECTIVITY: minReflectivity = getFloat(); break;
		case MINTRANSPARENCY:minTransparency = getFloat(); break;
		case MAXRECURSIONDEPTH: maxRecursionDepth = getFloat(); break;
		case MAKEPIXMAP: {	// get BMP file name for a pixmap
			// to be implemented, along the lines:
			int which = getFloat();// index of this pixmap in pixmap array
			if(which < 0 || which > 7){cout << "\nbad index of RGBpixmap!\n";}
			string fname = nexttoken(); // get file name for mesh
			if(!pixmap[which].readBMPFile(fname))
			{// read BMP file into this pixmap
			cout << " \ncan't read that RGBpixmap file!\n";
			return NULL;  }  
			break;}// end of case: MAKEPIXMAP
		case T_NULL: break; // The null token represents end-of-file
		case DEF: {
			string name, temp, lb, rb;
			int l = line;
			string inp;
			name = nexttoken();
			if ( whichtoken(name) != IDENT ) {
				cout << "Error:  Identifier expected." << endl;
				return NULL;
			}
			if ( def_stack->search(name) ) {
				cout << line << ": " << name;
				cout << ": attempt to redefine. " << endl;
				return NULL;
			}
			lb = nexttoken();
			if ( whichtoken(lb) != LFTCURLY ) {
				cout << "Error: { expected." << endl;
				return NULL;
			}
			while ( whichtoken( temp = nexttoken()) != RGHTCURLY ) {
				cout << temp << endl;
				inp = inp + temp + " ";
				if (!f_in) {
					cout << "Error: end of file detected." << endl;
					return NULL;
				}
			}
			// Push the contents of the string on the stack.
			def_stack->push(name, inp);
			break;} // end of case: DEF
		case USE: {
			string name;
			name = nexttoken();
			if ( whichtoken(name) != IDENT ) {
				cout << line << ": " << name;
				cout << ": identifier expected.";
				return NULL;
			}
			if (! def_stack->search(name) ) {
				cout << line << ": " << name;
				cout << ": not defined.";
				return NULL;
			}
			cout << def_stack->contents(name) << endl;
			strstream *temp_fin = new strstream;
			*temp_fin << def_stack->contents(name) << " ";
			*temp_fin << f_in->rdbuf();
			delete (f_in);
			f_in = temp_fin;
			break; } // end of case: USE
		default:  { // inner switch for Shapes
			switch(typ)
			{
				//float param;
			case CUBE:			newShape = new Cube;break;
			case SPHERE:		newShape = new Sphere;break;
			case TETRAHEDRON:	newShape = new Mesh("tetra.3vn");break;
			case TORUS:		newShape = new Torus;break;
			case PLANE:		newShape = new Plane;break;
			case SQUARE:		newShape = new Square;break;
			case TAPEREDCYLINDER:	newShape = new TaperedCylinder;
				((TaperedCylinder*)newShape)->smallRadius = getFloat(); break;
			case CONE:			newShape = new TaperedCylinder;
				((TaperedCylinder*)newShape)->smallRadius = 0; break;
			case CYLINDER:		newShape = new TaperedCylinder;
				((TaperedCylinder*)newShape)->smallRadius = 1; break;
			case TAPEREDTUBE:   newShape =new TaperedTube;
				((TaperedTube*)newShape)->smallRadius =getFloat();
				((TaperedTube*)newShape)->thickness =getFloat();
				if (((TaperedTube*)newShape)->thickness<0.0001) 
					((TaperedTube*)newShape)->thickness=0.0001;//make minimum thickness of tube=0.0001
		        //make thickess smaller than min(smallRaidus,1.0)
		        if (((TaperedTube*)newShape)->thickness>((TaperedTube*)newShape)->smallRadius
					|| ((TaperedTube*)newShape)->thickness>1.0 ) 
			    ((TaperedTube*)newShape)->thickness=(((TaperedTube*)newShape)->smallRadius<1.0)
				? ((TaperedTube*)newShape)->smallRadius:1.0;
				
				break;

			case OCTAHEDRON:		newShape = new Mesh("octa.3vn");break;
			case DODECAHEDRON:	newShape = new Mesh("dodeca.3vn"); break;
			case ICOSAHEDRON:	newShape = new Mesh("icosa.3vn"); break;
			case BUCKYBALL:		newShape = new Mesh("bucky.3vn");	break;
			case DIAMOND:		newShape = new Mesh("diamond.3vn"); break;
			case TEAPOT:		newShape = new Teapot; break;
			case MESH: {// get a filename (with extension) for this mesh
				string fname = nexttoken(); // get file name for mesh
				newShape = new Mesh(fname); break;
						  }// end of case: MESH
			default: {
				cerr << "Line " << nextline << ": unknown keyword " << s << endl;
				return NULL;
						}
			} // end of inner switch
			// common things to do to all Shape’s
			((Shape*)newShape)->mtrl.set(currMtrl);
			// load transform and its inverse
			((Shape*)newShape)->transf.set(*(affStk.tos->affn));
			((Shape*)newShape)->invTransf.set(*(affStk.tos->invAffn));
			return newShape;
					 }// end of default: block
    } // end of outer switch
  } // end of while
  return NULL;
} // end of getObject

// DefUnitStack methods
void DefUnitStack :: push(string n, string s) {
	D4S *temp_d4s = new D4S;
	temp_d4s->current = new DefUnit(n, s);
	temp_d4s->next = stack;
	stack = temp_d4s;
}
void DefUnitStack :: print() {
	D4S *temp = stack;
	string t;
	while (temp) {
		cout << temp->current->name << ":" ;
		cout << temp->current->stuff << endl;
		temp = temp->next;
	}
}
int DefUnitStack :: search(string s) {
	D4S *temp = stack;
	while (temp) {
		if ( temp->current->name == s ) {
			return(1);
		}
		temp = temp->next;
	}
	return(0);
}
string DefUnitStack :: contents(string s) {
	D4S *temp = stack;
	while (temp) {
		if (temp->current->name == s ) {
			return(temp->current->stuff);
		}
		temp = temp->next;
	}
	return(NULL);
}
void DefUnitStack :: release()
{
	while(stack)
	{
		D4S* tmp = stack; // grab it
		stack = stack->next; // advance p
		delete tmp->current; // release 2 strings
		delete tmp; // release node
	}
	stack = NULL;
}
//<<<<<<<<<<<<<<<<<<<<<<<< shade >>>>>>>>>>>>>>>>>>>>>>>>>>>
Color3 Scene :: shade(Ray& ray) // return the color of this ray
{
	Color3 color; //for ambient and diffuse with texturing
	Intersection inter, bestInter; // construct a new intersection record
	bestInter.numHits = 0; // no hits at the start

	for(GeomObj* pOb = obj; pOb != NULL; pOb = pOb->next)  // test each object in the scene
	{
		if(!pOb->hit(ray, inter)) // have a hit at a positive hit time?
			continue; // nope: a miss
		if(bestInter.numHits == 0 || inter.hit[0].hitTime < bestInter.hit[0].hitTime)
			bestInter.set(inter);  // copy inter into best
	} // end of replacement for getFirstHit()
	if(bestInter.numHits == 0)
	{
		return background; // no objects were hit
	}
	HitInfo h;
	h.set(bestInter.hit[0]); //copy first hit record of bestInter
	Point3 HitPoint(ray.start.x + ray.dir.x * h.hitTime,
						  ray.start.y + ray.dir.y * h.hitTime,
						  ray.start.z + ray.dir.z * h.hitTime);// form hitPoint by h.hitTime
	
	Vector3 v=ray.dir;
	v.flip();// direction to viewer*	
	v.normalize();// normalize v *
	
	Shape* myObj = (Shape*)h.hitObject; //point to hit object
	color.set(myObj->mtrl.emissive); //start with emissive part;
	Color3 ambientColor=ambient;
	ambientColor.multiply(myObj->mtrl.ambient);

	Point3 genHitP;
	xfrmPoint(genHitP,myObj->invTransf,HitPoint);
	//Color3 texture=myObj->mtrl.texture(genHitP);
	Color3 texr=texture(myObj,genHitP,h);

	ambientColor.multiply(texr);

	color.add(ambientColor);// adding ambient color;
	Vector3 normal;
	xfrmNormal(normal,myObj->invTransf,h.hitNormal);
	normal.normalize();
	for(Light* pLight=light;pLight!=NULL;pLight=pLight->next)
	{
//		if(isInShadow()) continue;
		Vector3 s;
		s.setDiff(pLight->pos,HitPoint);
		s.normalize();
		float mDotS=s.dot(normal);
		if(mDotS>0.0)
		{

			Color3 diffuseColor=myObj->mtrl.diffuse;
			diffuseColor.multiply(pLight->color);
			diffuseColor.multiply(mDotS);
			diffuseColor.multiply(texr);
			color.add(diffuseColor);
		}
		Vector3 h;
		h.set(s.x+v.x , s.y+v.y , s.z+v.z);
		h.normalize();
		float mDotH=h.dot(normal);
		if(mDotH>0.0)
		{
			float phong=pow(mDotH,myObj->mtrl.specularExponent);
			Color3 specColor=myObj->mtrl.specular;
			specColor.multiply(pLight->color);
			specColor.multiply(phong);
			color.add(specColor);
		}
	}

    if(color.red>1.0) color.red=1.0;
	if(color.green>1.0) color.green=1.0;
	if(color.blue>1.0) color.blue=1.0;
	return color;
} // end of Scene :: shade
//<<<<<<<<<<<<<<<<<<<<<<<< texture >>>>>>>>>>>>>>>>>>>>>>>>>>>
Color3 Scene :: texture(Shape* Obj,Point3 &p,HitInfo hit)
{
	Color3 txtColor, plainColor;
	Material curM=Obj->mtrl;//a handle copy
	switch (curM.textureType)
	{
	case 0: 
		txtColor.set(1,1,1);//nothing happnes
		break;
	case -1:{//checkerboard
		float S[3];
		for(int i=0;i<3;i++)//init S[3]; make sure nonzero
		{
			S[i]=(curM.params[i]>0.0)? curM.params[i]:0.25;
		}//S[i]=0.25 is default value
		// if there is no params, all S[i] are set to default value(0.25)
		// if for example 2 params only S(0) and S(1) are set new values,S(2) is still set
		// to 0.25,because the 3rd param is 0.0 by default.
		
		if(curM.numParams>=6)
		{ 
			//check params,make sure nonezero and smaller than 1.0
			if(curM.params[3]>1.0) curM.params[3]=1.0;
			if(curM.params[3]<0.0) curM.params[3]=0.0;
			if(curM.params[4]>1.0) curM.params[4]=1.0;
			if(curM.params[4]<0.0) curM.params[4]=0.0;
			if(curM.params[5]>1.0) curM.params[5]=1.0;
			if(curM.params[5]<0.0) curM.params[5]=0.0;
			plainColor.set(curM.params[3],curM.params[4],curM.params[5]);//use this color as default color
		}
		else
			plainColor.set(1.0,1.0,1.0);//use white as default color
	//make sure the ambient and diffuse reflection coefficient don't have zero component
		if(curM.ambient.red==0.0) curM.ambient.red=0.001;	
		if(curM.ambient.green==0.0) curM.ambient.green=0.001;
		if(curM.ambient.blue==0.0) curM.ambient.blue=0.001;

		if(curM.diffuse.red==0.0) curM.diffuse.red=0.001;
		if(curM.diffuse.green==0.0) curM.diffuse.green=0.001;
		if(curM.diffuse.blue==0.0) curM.diffuse.blue=0.001;
	//start the checkerboard jump function
		int jump;
	
		jump=((int)((p.x+1000)/S[0])+(int)((p.y+1000)/S[1])+(int)((p.z+1000)/S[2]))%2;//jump=0,1
		//adding 1000 to make sure nonneg values
		if(jump==1) txtColor.set(1,1,1);
			else txtColor.set(plainColor.red/curM.diffuse.red,plainColor.green/curM.diffuse.green,
			plainColor.blue/curM.diffuse.blue);
		//there is assumption that diffuse and ambient have the same rgb ratios
		//the idea here is if jump==0 the color is set to the plainColor and 
		//if jump==1 the color is set to the diffuse color
			break;}

	case -3:{//wood 	
		
		int rings;
		float M,K,N,B,D;//D is the offset of center from z-axis in x-axis
		switch(curM.numParams)
		{
		case 0:{
			 M=.1;K=2;N=PI*2;B=0.3;D=2;//default values if not specified
			 break;}
		case 1:{
			 M=curM.params[0];K=2;N=PI*2;B=0.3;D=2;
			 break;}
		case 2:{
			 M=curM.params[0];K=curM.params[1];N=PI*2;B=0.3;D=2;
			 break;}
		case 3:{
			 M=curM.params[0];K=curM.params[1];N=curM.params[2];B=0.3;D=2;
			 break;}
		case 4:{
			 M=curM.params[0];K=curM.params[1];N=curM.params[2];B=curM.params[3];D=2;
			 break;}
		case 5:{
			M=curM.params[0];K=curM.params[1];N=curM.params[2];
			B=curM.params[3];D=curM.params[4];
			 break;}
		default:{
			 M=.1;K=2;N=PI*2;B=0.3;D=2;
			 break;}
		}
		
		float r=sqrt((p.x+D)*(p.x+D)+p.y*p.y);
		float theta;
		if(p.z!=0.0) theta=atan(r/p.z);
		else theta=PI/2;

		if(curM.numParams>=8)
		{ 
			//check params,make sure nonezero and smaller than 1.0
			if(curM.params[6]>1.0) curM.params[6]=1.0;
			if(curM.params[6]<0.0) curM.params[6]=0.0;
			if(curM.params[7]>1.0) curM.params[7]=1.0;
			if(curM.params[7]<0.0) curM.params[7]=0.0;
			if(curM.params[5]>1.0) curM.params[5]=1.0;
			if(curM.params[5]<0.0) curM.params[5]=0.0;
			plainColor.set(curM.params[5],curM.params[6],curM.params[7]);//use this color as default color
		}
		else
			plainColor.set(1.0,1.0,1.0);//use white as default color
    //make sure the ambient and diffuse reflection coefficient don't have zero component
		if(curM.ambient.red==0.0) curM.ambient.red=0.001;	
		if(curM.ambient.green==0.0) curM.ambient.green=0.001;
		if(curM.ambient.blue==0.0) curM.ambient.blue=0.001;

		if(curM.diffuse.red==0.0) curM.diffuse.red=0.001;
		if(curM.diffuse.green==0.0) curM.diffuse.green=0.001;
		if(curM.diffuse.blue==0.0) curM.diffuse.blue=0.001;
	//start the checkerboard jump function
		rings=r/M+K*sin(theta/M+B*p.z)+10000;
		rings=((int)rings)%2;
		if(rings==1) txtColor.set(1,1,1);
		else txtColor.set(plainColor.red/curM.diffuse.red,plainColor.green/curM.diffuse.green,
			plainColor.blue/curM.diffuse.blue);
		break;}

	case -4:{//marble
		
		float marbleColor;
		marbleColor=Noi.marble(6,p);
		txtColor.set(marbleColor,marbleColor,marbleColor);
		break;}

	case 1://pixmap texture
    case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
		{
		float u,v;
		int i=curM.textureType;
		if(pixmap[i].nCols==0) 
		{
			txtColor.set(1,1,1);//if this pattern does not exist , ignore
			break;
		}
		float left,top,bottom,right;
		if(curM.numParams>=4)
		{
			left=curM.params[0];
			right=curM.params[1];
			top=curM.params[2];
			bottom=curM.params[3];
			if(abs(left)>1.0) left=-1.0;
			if(abs(right)>1.0) right=1.0;
			if(abs(bottom)>1.0) bottom=-1.0;
			if(abs(top)>1.0) top=1.0;
		}
		else
		{	
			left=-1.0;
			top=1.0;
			bottom=-1.0;
			right=1.0;
		};
	//make sure the ambient and diffuse reflection coefficient don't have zero component
		if(curM.ambient.red==0.0) curM.ambient.red=0.001;	
		if(curM.ambient.green==0.0) curM.ambient.green=0.001;
		if(curM.ambient.blue==0.0) curM.ambient.blue=0.001;

		if(curM.diffuse.red==0.0) curM.diffuse.red=0.001;
		if(curM.diffuse.green==0.0) curM.diffuse.green=0.001;
		if(curM.diffuse.blue==0.0) curM.diffuse.blue=0.001;
	//start the pixmap mapping
        float ix,iy;
		switch(Obj->Tag)
		{
		case 1:
			{
			ix=p.x;
			iy=p.y;
			
			break;
			}
		case 2:
			{	
				Vector3 norm=hit.hitNormal;//look for which surface to draw
				
				if((float)p.z==1.0){ix=p.x;iy=p.y;}
				else if((float)p.z==-1.0){ix=-p.x;iy=p.y;}
				if(norm.z==1){ix=p.x;iy=p.y;}
				else if(norm.z==-1){ix=-p.x;iy=p.y;}

				//else if(abs(p.x-1.0)<0.0001){ix=-p.z;iy=p.y;cout<<"x=1"<<"\t\t";}
				else if(norm.x==1){ix=-p.z;iy=p.y;}
				else if(norm.x==-1){ix=p.z;iy=p.y;}
				else if(norm.y==1){ix=p.x;iy=-p.z;}
				else if(norm.y==-1){ix=-p.x;iy=p.z;}
				else {cout<<p.x<<"\t"<<p.y<<"\t"<<p.z<<"\n";};//ix=p.x;iy=top*1000;cout<<"p.x="<<ix<<"\t\t";};//will ignore the bmp color
				break;
			}
		case 3:
			{
				if(p.x!=0.0)
					ix=atan(p.y/p.x);
				else
					ix=PI/2;
				iy=p.z;
				break;
			}
		default:
			{
				//ix=right*1000;iy=top*1000;
				break;
			}
		};			
	    u=(ix-left)/(right-left);
		v=(iy-bottom)/(top-bottom);
		if(u<0.0 ||u>=1.0 ||v<0.0 ||v>=1.0)
		{
			txtColor.set(1.0,1.0,1.0);
			//cout<<"outrange"<<"\t\t";
			break;
		}
		

		int N=pixmap[i].nCols, M=pixmap[i].nRows;
		mRGB rgb=pixmap[i].getPixel((int)(u*N),(int)(v*M));
		txtColor.set(rgb.r/curM.diffuse.red/256,rgb.g/curM.diffuse.green/256,
			rgb.b/curM.diffuse.blue/256);

		break;
		}

	default:
		txtColor.set(1,1,1);
		break;
		
	}
	return txtColor;
};//end of  Scene :: texture(Point3 &p)

//<<<<<<<<<<<<<<<<<<<<<<<< noise >>>>>>>>>>>>>>>>>>>>>>>>>>>
float Noise:: noise(float scale, Point3& p)
{// linearly interpolated lattice noise
#define lerp(f, A, B) A+f*(B-A)
	float d[2][2][2];
	Point3 pp;
	pp.x=p.x*scale+10000;
	pp.y=p.y*scale+10000;
	pp.z=p.z*scale+10000;
	long ix=(long)pp.x;
	long iy=(long)pp.y;
	long iz=(long)pp.z;
	float tx,ty,tz,x0,x1,x2,x3,y0,y1;
	tx=pp.x-ix;
	ty=pp.y-iy;
	tz=pp.z-iz;
	//float mtx=1.0-tx,mty=1.0-ty,mtz=1.0-tz;
	for(int k=0;k<=1;k++)
		for(int j=0;j<=1;j++)
			for(int i=0;i<=1;i++)
				d[k][j][i]=latticeNoise(ix+i,iy+j,iz+k);
	x0=lerp(tx,d[0][0][0],d[0][0][1]);
	x1=lerp(tx,d[0][1][0],d[0][1][1]);
	x2=lerp(tx,d[1][0][0],d[1][0][1]);
	x3=lerp(tx,d[1][1][0],d[1][1][1]);
	y0=lerp(ty,x0,x1);
	y1=lerp(ty,x2,x3);
	return lerp(tz,y0,y1);
}//end of noisefloat scale, Point3& p)
//<<<<<<<<<<<<<<<<<<<<<<<< turbulence >>>>>>>>>>>>>>>>>>>>>>>>>>>
float Noise::turbulence(float s,Point3& p)
{
	//int M=3;
	float turb=0;
	for(int i=0;i<=M;i++)
	{
		turb=turb+1.0/pow(2.0,i)*noise(pow(2.0,i)*s,p);
	}
	turb=turb/2.0;
	//turb=noise(s,p);
	
	return turb;
}
//end of turbulence( scale, Point3& p)
//<<<<<<<<<<<<<<<<<<<<<<<< marble >>>>>>>>>>>>>>>>>>>>>>>>>>>
float Noise::marble(float strength, Point3& p)
{
	return undulate(sin(2*PI*p.z+strength*turbulence(5,p)));
    //return turbulence(5,p);
}
//end of marble(A, Point3& p)