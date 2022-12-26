// SDL.h  - adjusted for ECE661 to admit simple raytracing
// definition of simple support classes
// students must implement the hit(), and drawOpenGL() methods of some shapes
#ifndef _SDLStuff
#define _SDLStuff
#include <string>
#include <iostream>
#include <fstream>
#include <strstream>
using namespace std;

#include <windows.h>
#include <assert.h>
#include <math.h>
#include <gl/Gl.h>
#include <gl/Glu.h>
#include <gl/glut.h>

#include "RGBpixmap.h"
#define PI 3.14159265
class Ray; // declare these classes' existence
class Intersection;
//@@@@@@@@@@@@@@@@@@ Point3 class @@@@@@@@@@@@@@@@
class Point3{
public:
	float x,y,z;
	void set(float dx, float dy, float dz){x = dx; y = dy; z = dz;}
	void set(Point3& p){x = p.x; y = p.y; z = p.z;}
 Point3(float xx,     float yy, float zz){x = xx; y = yy; z = zz;}
	Point3(){x = y = z = 0;}
	void build4tuple(float v[])
	{// load 4-tuple with this color: v[3] = 1 for homogeneous
		v[0] = x; v[1] = y; v[2] = z; v[3] = 1.0f;
	}
};
//@@@@@@@@@@@@@@@@@@ Vector3 class @@@@@@@@@@@@@@@@
class Vector3{
public:
	float x,y,z;
	void set(float dx, float dy, float dz){ x = dx; y = dy; z = dz;}
	void set(Vector3& v){ x = v.x; y = v.y; z = v.z;}
	void flip(){x = -x; y = -y; z = -z;} // reverse this vector
	void setDiff(Point3& a, Point3& b)//set to difference a - b
	{ x = a.x - b.x; y = a.y - b.y; z = a.z - b.z;}
	void normalize();//adjust this vector to unit length
	Vector3(float xx, float yy, float zz){x = xx; y = yy; z = zz;}
	Vector3(Vector3& v){x = v.x; y = v.y; z = v.z;}
	Vector3(){x = y = z = 0;} //default constructor
	Vector3 cross(Vector3 b); //return this cross b
	float dot(Vector3 b); // return this dotted with b
};
// @@@@@@@@@@@@@@@@@@@@@ Color3 class @@@@@@@@@@@@@@@@
class Color3 { // holds an red,green,blue 3-tuple
public:
	float red, green, blue;
	Color3(){red = green = blue = 0;}
	Color3(float r, float g, float b){red = r; green = g; blue = b;}
	Color3(Color3& c){red = c.red; green = c.green; blue = c.blue;}
	void set(float r, float g, float b){red = r; green = g; blue = b;}
	void set(Color3& c)
      {red = c.red; green = c.green; blue = c.blue;}
	void add(float r, float g, float b)
      {red += r; green += g; blue += b;}
	void add(Color3& src, Color3& refl);
	void add(Color3& colr);
	void build4tuple(float v[]);
	void multiply(Color3 ref) // return this dotted with ref
	{red *=ref.red; green *=ref.green; blue *=ref.blue;}
	void multiply(float ref) // return this dotted with ref
	{red *=ref; green *=ref; blue *=ref;}

};
//@@@@@@@@@@@@@@@@@@@@ light class @@@@@@@@@@@@@@@@@@@
class Light{ // for a linked list of light sources’ color and position
public:
	Point3 pos;
	Color3 color;
	Light* next;
	void setPosition(Point3 p){pos.set(p);}
	void setColor(Color3 c){color.set(c);}
	Light(){next = NULL;}
};
// @@@@@@@@@@@@@@@@@@@@@ Affine4 class @@@@@@@@@@@@@@@@
class Affine4 {// manages homogeneous affine transformations
// including inverse transformations
// and a stack to put them on
// used by Scene class to read SDL files
public:
	float m[16]; // hold a 4 by 4 matrix
	Affine4();
	void setIdentityMatrix();
	void set(Affine4 a);
	void preMult(Affine4 n);
	void postMult(Affine4 n);
}; // end of Affine4 class

//@@@@@@@@@@ AffineNode class @@@@@@@@@@@
class AffineNode{
// used by Scene class to read SDL files
public:
	Affine4 * affn;
	Affine4 * invAffn;
	AffineNode * next;
	AffineNode()
	{
		next = NULL;
		affn = new Affine4; // new affine with identity in it
		invAffn = new Affine4; // and for the inverse
	}
	~AffineNode() //destructor
	{
		delete affn;
		delete invAffn;
	}
};
//@@@@@@@@@@@@@@@@ AffineStack class @@@@@@@@@@@@
class AffineStack{
// used by Scene class to read SDL files
public:
	AffineNode * tos;
	AffineStack()//default constructor;puts identity on top
	{
		tos = new AffineNode; // node with identity in it
		tos->next = NULL;
	}
	void dup();
	void setIdentity();// make top item the identity matrix
	void popAndDrop();
	void releaseAffines(); // pop and drop all remaining items
	void rotate(float angle, Vector3 u);
	void scale(float sx, float sy, float sz);
	void translate(Vector3 d);
}; // end of AffineStack class
//@#@#@#@#@#@#@#@# Noise class @#@#@#@#@#@#@#@#
class Noise{
public:
	Noise()//constructor
	{
		M=3;
		int i;
		index=new unsigned char[256];
		for(i=0;i<256;i++) index[i]=i;//fill array with indices
		for(i=0;i<256;i++) //shuffle it
		{
			int which=rand()%256;//choose randowm place in array
			unsigned char tmp=index[which];//swap them
			index[which]=index[i];
			index[i]=tmp;
		}
		noiseTable= new float[256];
		for(i=0;i<256;i++) noiseTable[i]=rand()/32767.99;
	}//end of constructor
	/*Noise(int x)//another constructor to sepcify M value
	{
		M=x;
		int i;
		index=new unsigned char[256];
		for(i=0;i<256;i++) index[i]=i;//fill array with indices
		for(i=0;i<256;i++) //shuffle it
		{
			int which=rand()%256;//choose randowm place in array
			unsigned char tmp=index[which];//swap them
			index[which]=index[i];
			index[i]=tmp;
		}
		noiseTable= new float[256];
		for(i=0;i<256;i++) noiseTable[i]=rand()/32767.99;
	}//end of constructor*/
	float noise(float x,float y,float z);
	float noise(float scale,Point3& p);
	float turbulence(float s, Point3& p);
	float marble(float x,float y,float z);
	float marble(float strength,Point3& p);
	float undulate(float x)
	{
		if(x<-0.4) return 0.15+2.857*pow(x+0.75,2);
		else if(x<0.4) return 0.95-2.8125*pow(x,2);
		else return 0.26+2.666*pow(x-0.7,2);
	}
private:
	float* noiseTable;   // array of noise values
	unsigned char * index;  //pseudo random indices
	float mySpline(float x);   //used for marble
	int M;
	float latticeNoise(int i, int j, int k)
	{//return noise value on an interger lattice
        #define PERM(x) index[(x) & 255]
		#define INDEX(ix,iy,iz) PERM((ix)+PERM((iy)+PERM(iz)))
		return noiseTable[INDEX(i,j,k)];
	}
};//end of noise class
//@@@@@@@@@@@@@@@@@ Material class @@@@@@@@@@@@@@
class Material{
public:
	Color3 ambient, diffuse, specular, emissive;
    //Noise n;
	int numParams; // for textures
	float params[10]; // for textures
	int textureType;  // 0 for none, neg for solids, pos for images

	float specularExponent, reflectivity, transparency, speedOfLight;
	float specularFraction, surfaceRoughness;
	void setDefault();
	void set(Material& m);

	//Color3 texture(Point3 &p);
	

}; // end of Material

//@@@@@@@@@@@@@@@@@@@@@ GeomObj class @@@@@@@@@@@@@@@@
class GeomObj{
public:
	//IntRect scrnExtnt;
	GeomObj * next;
	GeomObj(): next(NULL){}
	virtual bool hit(Ray &r, Intersection &inter){return false;}
	virtual void loadStuff(){}
	virtual void drawOpenGL(){}
	virtual void tellMaterialsGL(){}
};
//@@@@@@@@@@@@@@@@@@@ Boolean @@@@@@@@@@@@@@@@@@
class Boolean: public GeomObj{
public:
	GeomObj *left, *right;
	Boolean():left(NULL),right(NULL){}
	virtual bool hit(Ray &r, Intersection &inter){return false;}
	virtual void drawOpenGL()
	{ // just draw its children
		if(left)left->drawOpenGL();
		if(right)right->drawOpenGL();
	}
};
//@@@@@@@@@@@@@@@@@@@@ UnionBool @@@@@@@@@@@@@@@@
class UnionBool : public Boolean{
public:
	UnionBool(){Boolean();} //constructor
	bool hit(Ray &r, Intersection &inter){return false;}
};
//@@@@@@@@@@@@@@@@@@@@ IntersectionBool @@@@@@@@@@@@@@@@
class IntersectionBool : public Boolean{
public:
	IntersectionBool(){Boolean();}
	bool hit(Ray &r, Intersection &inter){return false;}
};
//@@@@@@@@@@@@@@@@@@@@ DifferenceBool @@@@@@@@@@@@@@@@
class DifferenceBool : public Boolean{
public:
	DifferenceBool(){Boolean();}
	bool hit(Ray &r, Intersection &inter){return false;}
};
//@@@@@@@@@@@@@@@@@ Shape @@@@@@@@@@@@@@@@@@@@@
class Shape: public GeomObj{
public:
	Material mtrl;
	Affine4 transf,invTransf;
	int Tag;
	//virtual Color3 texture(HitInfo& h, int whichTexture);
	Shape(){mtrl.textureType = 0; mtrl.numParams = 0;}
	void setMaterial(Material& mt){mtrl.set(mt);}
	void tellMaterialsGL();
	virtual void drawOpenGL(){}
	virtual bool hit(Ray &r, Intersection &inter){return false;}

}; //end: Shape class

//@$@$@$@$@$@$@$@$@ Cube class $@$@$@$@$@$@$@$@$@$@
class Cube : public Shape{
public:
	Cube(){Tag=2;}
	void drawOpenGL()
	{
		tellMaterialsGL(); glPushMatrix();
		glMultMatrixf(transf.m); //load affine
		glEnable(GL_NORMALIZE);
		glutSolidCube(2.0);	// a cube with vertices -1 to +1
		glPopMatrix();
	}
	bool hit(Ray &r, Intersection &inter);
	Vector3 cubeNormal(int surface);
};
//@$@$@$@$@$@$@$@$ Sphere class @$@$@$@$@$@$@$@$@$@$@$@
class Sphere : public Shape{
public:
	void drawOpenGL()
	{
		tellMaterialsGL(); 	glPushMatrix();
		glMultMatrixf(transf.m);
		glutSolidSphere(1.0,20,20);
		glPopMatrix();
	}
	Sphere() { }
	bool hit(Ray &r, Intersection &inter);
};
//@$@$@$@$@$@$@$@$@$@ TaperedCylinder class @$@$@$@$@$@$@$@$@$
class TaperedCylinder : public Shape{
public:
		float smallRadius;
		
		TaperedCylinder(){Tag=3;}
		void drawOpenGL()
		{ 
			tellMaterialsGL(); 	glPushMatrix();
			glMultMatrixf(transf.m);

			GLUquadricObj *quadObj = gluNewQuadric();
	        gluQuadricDrawStyle(quadObj, (GLenum) (GLU_FILL));
	
	        gluQuadricNormals(quadObj, (GLenum) GLU_SMOOTH);
			gluQuadricOrientation(quadObj, (GLenum) GLU_OUTSIDE);
			
			gluCylinder(quadObj, 1.0, smallRadius, 1.0,30,10);

			gluQuadricOrientation(quadObj, (GLenum) GLU_INSIDE);
			gluDisk(quadObj,0.0,1.0,20,20);
			glTranslatef(0.0,0.0,1.0);
			glRotatef(180,0.0,1.0,0.0);
			gluDisk(quadObj,0.0,smallRadius,30,10);
			gluDeleteQuadric(quadObj);
			glPopMatrix();

			/* to be implemented */
		}
		bool hit(Ray &r, Intersection &inter);//{return false;}
};
//@$@$@$@$@$@$@$@$@$@ TaperedCylinder class @$@$@$@$@$@$@$@$@$
class TaperedTube : public Shape{
public:
		float smallRadius,thickness;
		
			
		TaperedTube(){}
		void drawOpenGL()
		{ 
			//cout<<"thickness"<<thickness;
			tellMaterialsGL(); 	glPushMatrix();
			glMultMatrixf(transf.m);

			GLUquadricObj *quadObj = gluNewQuadric();
            //inner shell
			gluQuadricDrawStyle(quadObj, (GLenum) (GLU_FILL));	
	        gluQuadricNormals(quadObj, (GLenum) GLU_SMOOTH);
			gluQuadricOrientation(quadObj, (GLenum) GLU_INSIDE);			
			gluCylinder(quadObj, 1.0-thickness, smallRadius-thickness, 1.0,30,10);
			//gluQuadricOrientation(quadObj, (GLenum) GLU_OUTSIDE);
            //outside shell
	        gluQuadricDrawStyle(quadObj, (GLenum) (GLU_FILL));	
	        gluQuadricNormals(quadObj, (GLenum) GLU_SMOOTH);
			gluQuadricOrientation(quadObj, (GLenum) GLU_OUTSIDE);			
			gluCylinder(quadObj, 1.0, smallRadius, 1.0,30,10);
			gluQuadricOrientation(quadObj, (GLenum) GLU_INSIDE);
			//top and base
			gluDisk(quadObj,1-thickness,1.0,30,10);
			glTranslatef(0.0,0.0,1.0);
			glRotatef(180,0.0,1.0,0.0);
			gluDisk(quadObj,smallRadius-thickness,smallRadius,30,10);
			gluDeleteQuadric(quadObj);
			glPopMatrix();

			/* to be implemented */
		}
		bool hit(Ray &r, Intersection &inter);
};
//end of TaperedTube class
//@$@$@$@$@$@$@$@$@$@ Square class @$@$@$@$@$@$@$@$@$
class Square : public Shape{
public:
		Square(){Tag=1;}
		void drawOpenGL()
		{
			tellMaterialsGL(); 	glPushMatrix();
			glMultMatrixf(transf.m);

			glRectf(-1.0,-1.0,1.0,1.0);
			glPopMatrix();

			/* to be implemented */
		}
		bool hit(Ray &r, Intersection &inter);
};
//@$@$@$@$@$@$@$@$@$@ Plane class @$@$@$@$@$@$@$@$@$
class Plane : public Shape{
public:
		Plane(){}
		void drawOpenGL(){ /* to be implemented */}
		bool hit(Ray &r, Intersection &inter){return false;}
};
//################## class VertexID ################
//used to define a Mesh
class VertexID{public: int vertIndex, normIndex;};
//################# class FACE ##############
//used to define a Mesh
class Face{
public:
		int nVerts;
		VertexID * vert; // array of vertex and normal indices
		Face(){ nVerts = 0; vert = NULL;}
		~Face(){delete[] vert; nVerts = 0;}
};
//@$@$@$@$@$@$@$@$@$@ Mesh class @$@$@$@$@$@$@$@$@$
class Mesh : public Shape{
private:
		int numVerts, numNorms, numFaces;
		Point3 *pt; // array of points
		Vector3 *norm; // array of normals
		Face *face; // array of faces
		int lastVertUsed;
		int lastNormUsed;
		int lastFaceUsed;
public:
		void readMesh(string fname);
		void writeMesh(char* fname);
		void printMesh();
		void drawMesh();
		void drawEdges();
		void freeMesh();
		int isEmpty();
		void makeEmpty();
		Mesh();
		virtual void drawOpenGL();
		Mesh(string fname);
		Vector3 newell4(int indx[]);
		bool hit(Ray &r, Intersection &inter);
		string meshFileName; // holds file name for this Mesh
}; // end of Mesh class
//@$@$@$@$@$@$@$@$@$@ Torus class @$@$@$@$@$@$@$@$@$
class Torus : public Shape{
public:
		void drawOpenGL(){
			tellMaterialsGL(); 	glPushMatrix();
			glMultMatrixf(transf.m);
			glutSolidTorus(0.2,1.0,10,12);
			glPopMatrix();}
			bool hit(Ray &r, Intersection &inter){return false;}
};
//@$@$@$@$@$@$@$@$@$@ Teapot class @$@$@$@$@$@$@$@$@$
class Teapot : public Shape{
public:
		void drawOpenGL(){ tellMaterialsGL(); glPushMatrix();
		glMultMatrixf(transf.m);
		glutSolidTeapot(1.0); glPopMatrix();}
		bool hit(Ray &r, Intersection &inter){return false;}
};
//@@@@@@@@@@@@@@@@@ DefUnit & DefUnitStack classes @@@@@@@@@@@@@@
//used in Scene to read SDL files
class DefUnit {
	// developed by Steve Morin
public:
	string    name, stuff;
	DefUnit(string n, string s) {stuff = s;name = n;}
};
class DefUnitStack {
public:
	DefUnitStack() {stack = NULL;}
	void push(string n, string s);
	void print();
	int search(string s);
	string contents(string s);
	void release();
private:
	struct D4S {
		DefUnit *current;
		struct D4S *next;
	} d4s;
	D4S *stack;
}; // end of DefUnitStack class

//+++++++++++++ TokenType +++++++++++++
		enum mTokenType {IDENT, LIGHT, ROTATE, TRANSLATE,
		SCALE, PUSH, POP, IDENTITYAFFINE,
		GLOBALAMBIENT, BACKGROUND, MINREFLECTIVITY,
		MINTRANSPARENCY, MAXRECURSIONDEPTH, CUBE, SPHERE, TORUS, PLANE,
		SQUARE, CYLINDER, CONE, TAPEREDCYLINDER,TAPEREDTUBE,TETRAHEDRON, OCTAHEDRON,
		DODECAHEDRON, ICOSAHEDRON,BUCKYBALL, TEAPOT,
		DIAMOND,UNION,INTERSECTION, DIFFERENCEa, MAKEPIXMAP,
		MESH, DEFAULTMATERIALS, AMBIENT, DIFFUSE,SPECULAR,
		SPECULARFRACTION, SURFACEROUGHNESS,EMISSIVE, SPECULAREXPONENT,
		SPEEDOFLIGHT, TRANSPARENCY,REFLECTIVITY, PARAMETERS, TEXTURE,
		LFTCURLY, RGHTCURLY, DEF, USE, T_NULL, F_EOF, UNKNOWN };
class HitInfo;
//@@@@@@@@@@@@@  Scene class @@@@@@@@@@@@@@@@@@@@
class Scene{
public:
	Light *light;  // attach linked list of lights here
	GeomObj * obj;  // attach the object list here
	RGBpixmap pixmap[8];
	Color3 background, ambient;
	int maxRecursionDepth;
	Noise Noi;
	//must #include RGBpixmap.h to have following texture fields
	//RGBpixmap pixmap[8]; //list of attached pixmaps
	float minReflectivity, minTransparency;
	//bool isInShadow(Ray& f); // for ray tracing: implementation left to the reader
	Scene():light(NULL),obj(NULL),tail(NULL) //default constructor
	{
		currMtrl.setDefault();
		background.set(0,0,0.6f);
		ambient.set(0.1f,0.1f,0.1f);
		minReflectivity = 0.5;
		minTransparency = 0.5;
		maxRecursionDepth = 3;
	}
	Scene(string fname){Scene(); read(fname);}
	void freeScene();
	void makeLightsOpenGL(){/* to be implemented */}
	void drawSceneOpenGL();
	bool read(string fname);
	GeomObj* getObject();
	Color3 shade(Ray& ray);
	Color3 texture(Shape* Obj,Point3 &p,HitInfo hit);

private:
	// private stuff used only for reading a scene
	int line;
	int nextline;
	ifstream  *file_in;
	strstream *f_in;
	strstream temp_fin;
	DefUnitStack *def_stack;
	GeomObj * tail; // tail of object list
	AffineStack affStk; // affine stack
	Material currMtrl;
	string nexttoken(void);
	float getFloat();
	bool isidentifier(string keyword);
	void cleanUp();
	mTokenType whichtoken(string keyword);
};
class Intersection;
#define MaxNumInside 10
//@#@#@#@#@#@#@#@# Ray class @#@#@#@#@#@#@#@#@#
class Ray{
public:
	Point3 start;
	Vector3 dir;
	int recurseLevel;
	int row, col; // for screen extents
	int numInside; // number of objects on list
	GeomObj* inside[MaxNumInside]; // array of object pointers
	Ray(){start.set(0,0,0); dir.set(0,0,0); numInside = 0;}
	Ray(Point3 origin); //constructor: set start point of ray
	Ray(Point3& origin, Vector3& direction)
		{ start.set(origin); dir.set(direction); numInside = 0;} //constructor
	void setStart(Point3& p){start.set(p);}
	void setDir(float x, float y, float z)
	{dir.x = x; dir.y = y; dir.z = z;}
	void setRayDirection(Light *L); //for shadow feelers
	void setRayDirection(Vector3& dir); //for reflected and transmitted rays
	int isInShadow();
	void getHitParticulars(Intersection& inter,Point3& hitPoint,Vector3& normal);
	void makeGenericRay(GeomObj* p, Ray& gr);
};
//@#@#@#@#@#@#@#@#  HitInfo class @#@#@#@#@#@#@#@#
class HitInfo { // data for each hit with a surface
   public:
	double hitTime;    		// the hit time
	GeomObj* hitObject;  	// the object hit
	int surface;			    // which surface is hit?
	int isEntering;			 // is ray entering the object?
	Point3 hitPoint;		   // hit point
	Vector3 hitNormal;		// normal at hit point
	HitInfo()
	{
		hitObject = NULL; hitTime = -1000; surface = 0; isEntering = 0;
	} // constructor
	void set(HitInfo& h)
	{
		hitTime = h.hitTime; hitObject = h.hitObject;	surface = h.surface;
		isEntering = h.isEntering;hitPoint.set(h.hitPoint); hitNormal.set(h.hitNormal);
	}
};
#define maxNumHits 8
//@#@#@#@#@#@#@#@# Intersection class @#@#@#@#@#@#@#@#
class Intersection{ // hold hit list
   public:
	int numHits;									// the number of hits
	HitInfo hit[maxNumHits];			// list of hits;
	Intersection(){numHits = 0;}  // default constructor
	void set(Intersection& intr)
	{ // copy intersection info
		numHits = intr.numHits;
		for(int i = 0; i < maxNumHits; i++)
			hit[i].set(intr.hit[i]);
	}
};

void xfrmPoint(Point3& res, Affine4& aff,float x, float y, float z);
Point3 rayPos(Point3& s, Vector3& v, float t);
Point3 rayPos(Ray&r, float t);
void xfrmPoint(Point3& res, Affine4& aff,Point3& pt);
void xfrmVect(Vector3& res, Affine4& aff,Vector3& v);
void xfrmRay(Ray& g, Affine4& aff, Ray& r);
void xfrmNormal(Vector3& res, Affine4& aff,Vector3& v);
// end of SDL.h
#endif



