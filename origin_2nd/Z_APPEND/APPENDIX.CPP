//Appendix 3. Some useful Classes and Utility Routines.
//This appendix defines some basic types and algorithms that may prove useful as you develop graphics applications as described throughout the book. The basic types are developed through classes which can be used at will.

//Some of the classes are fully developed here. Others have a number of methods declared but not defined: It is up to the student to flesh these out. Some classes are given here in very skeletal form, to suggest what might be developed in an actual application.

//In most classes a relaxed approach to encapsulation is taken: most data fields are declared public rather than private, as a matter of brevity and to avoid the need to define a large number of accessor and mutator functions.

//Another excellent source of classes and utilities may be found in the Graphics Gems series, whose on-line repository is http://www.acm.org/tog/GraphicsGems/index.html.

//Classes for 2D graphics.
// definition of simple support classes:
#include <string>
#include <iostream>
#include <fstream>
#include <strstream>
using namespace std;

#include <windows.h> //change if using xWindows
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <gl/Gl.h>
#include <gl/Glu.h>
#include <gl/glut.h>

//@@@@@@@@@@@@@@@@@@ IntPoint class @@@@@@@@@@@@@@@@
class IntPoint{ // for 2D points with integer coordinates
public: 
	int x,y;
	void set(int dx, int dy){x = dx; y = dy;}
	void set(IntPoint& p){ x = p.x; y = p.y;} 
	IntPoint(int xx, int yy){x = xx; y = yy;}
	IntPoint(){ x = y = 0;}
};
//@@@@@@@@@@@@@@@@@@ Point2 class @@@@@@@@@@@@@@@@
class Point2{ // for 2D points with real coordinates
public: 
	float x,y;
	void set(float dx, float dy){x = dx; y = dy;}
	void set(Point2& p){ x = p.x; y = p.y;} 
	Point2(float xx, float yy){x = xx; y = yy;}
	Point2(){x = y = 0;}
};
//<<<<<<<<<<<<<<<<<<<<<< PolyLine >>>>>>>>>>>>>>>>>>>>>>>>>
class PolyLine{ // a polyline is a num plus an array of points
public:
	int num;
	Point2 pt[80]; //may need larger arrays in some circumstances
	PolyLine(){num = 0;}
};
// @@@@@@@@@@@@@@@@@@@@@@@@ IntRect class @@@@@@@@@@@@@@@@@@@@
class IntRect{ // a rectangle with integer border values
public: 
  int left, top, right, bott;
  IntRect(){left = top = right = bott = 0;}
  IntRect(int l, int t, int r, int b)
  {left = l; top = t; right = r; bott = b;}
  void set(int l, int t, int r, int b)
  {left = l; top = t; right = r; bott = b;}
  void set(IntRect& r)
  {left = r.left; top = r.top; right = r.right; bott = r.bott;}
};
//@@@@@@@@@@@@@@@@@@ Vector2 class @@@@@@@@@@@@@@@@
class Vector2{ 
public: 
	float x,y;
	void set(float dx, float dy){ x = dx; y = dy; } 
	void set(Vector2& v){ x = v.x; y = v.y;}
	void setDiff(Point2& a, Point2& b)//set to difference a - b
	{x = a.x - b.x; y = a.y - b.y;}
	void normalize()//adjust this vector to unit length
	{		double sizeSq = x * x + y * y;
		if(sizeSq < 0.0000001)
		{
			cerr << "\nnormalize() sees vector (0,0)!";
			return; // does nothing to zero vectors;
		}
		float scaleFactor = 1.0/(float)sqrt(sizeSq);
		x *= scaleFactor; y *= scaleFactor;
	}
	Vector2(float xx, float yy){x = xx; y = yy; }
	Vector2(Vector2& v){x = v.x; y = v.y; }
	Vector2(){x = y = 0;} //default constructor
	float dot(Vector2 b) // return this dotted with b
 {return x * b.x + y * b.y;}
	void perp() // perp this vector
	{float tmp = x; x = -y; y = tmp;}
	float perpDot(Vector2& v) // return perp of this dotted with v
	{return x *v.x - y * v.y;}
};
//<<<<<<<<<<<<<<<<<<<< Canvas class >>>>>>>>>>>
// a global Canvas object (described in Chapter 3) knows how 
//to draw lines in world coordinates and to perform turtlegraphics
class Canvas {
 private:
	  Point2 CP; 	// current position in world
	  float CD; // current direction in degrees
 public:
	  float windowAspect;
	  Canvas(int width, int height, char* title)
	  {
		char* list; //dummy list for glutInit
		int numArgs = 1;//dummy value for glutInit
		glutInit(&numArgs, &list);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
		glutInitWindowSize(width, height);
		glutInitWindowPosition(100, 100);
		glutCreateWindow(title);
		CP.x = CP.y = 0.0;
		windowAspect = 1.0;
	}
	void setWindow(float l, float r, float b, float t)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D((GLdouble)l, (GLdouble)r, (GLdouble)b, (GLdouble)t);
		if(t == b) return;
		windowAspect = (r - l)/(t - b);
	}
	void setViewport(int l, int r, int b, int t)
		{glViewport((GLint)l, (GLint)b, (GLint)(r-l), (GLint)(t-b));}	
	float getWindowAspect(void) { return windowAspect;}
	void lineTo(float x, float y)
	{
		glBegin(GL_LINES);
		glVertex2f((GLfloat)CP.x, (GLfloat)CP.y);
		CP.x = x; CP.y = y;
		glVertex2f((GLfloat)CP.x, (GLfloat)CP.y);
		glEnd(); 	glFlush();
	}
	void moveTo(float x, float y){CP.x = x; CP.y = y;}
	void turn(float ang) {CD += ang;}
	void turnTo(float ang) {CD = ang;}
	void forward(float dist, int vis)	
	{
		#define RadPerDeg 0.017453393 //radians per degree
		float x = CP.x + dist * cos(RadPerDeg * CD);
		float y = CP.y + dist * sin(RadPerDeg * CD);
		if(vis) lineTo(x, y);
		else moveTo(x, y);
		CP.x = x; CP.y = y;
	}
	void initCT() // initialize the CT (model view matrix)
	{
		glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	}
	void rotate2D(double angle)
	{
		glMatrixMode(GL_MODELVIEW); glRotated(angle, 0.0, 0.0, 1.0);	
  }
	void translate2D(double dx, double dy)
	{
		glMatrixMode(GL_MODELVIEW); glTranslated(dx, dy, 0.0);
	}
	void scale2D(double sx, double sy)
	{
		glMatrixMode(GL_MODELVIEW); glScaled(sx, sy, 1.0);
	}
	void pushCT(void)
	{
		glMatrixMode(GL_MODELVIEW); glPushMatrix();
	}
	void popCT(void)
	{
		glMatrixMode(GL_MODELVIEW); glPopMatrix();
	}
	void ngon(int n, float cx, float cy, float radius);
};
//3D Classes for Graphics

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
	void normalize()//adjust this vector to unit length
	{		
		double sizeSq = x * x + y * y + z * z;
		if(sizeSq < 0.0000001)
		{
			cerr << "\nnormalize() sees vector (0,0,0)!";
			return; // does nothing to zero vectors;
		}
		float scaleFactor = 1.0/(float)sqrt(sizeSq);
		x *= scaleFactor; y *= scaleFactor; z *= scaleFactor;
	}
	Vector3(float xx, float yy, float zz){x = xx; y = yy; z = zz;}
	Vector3(Vector3& v){x = v.x; y = v.y; z = v.z;}
	Vector3(){x = y = z = 0;} //default constructor
	Vector3 cross(Vector3 b) //return this cross b
	{
	   Vector3 c(y*b.z - z*b.y, z*b.x - x*b.z, x*b.y - y*b.x);
	   return c;
	}
	float dot(Vector3 b) // return this dotted with b
 {return x * b.x + y * b.y + z * b.z;}
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
	void add(Color3& src, Color3& refl)
	{ // add the product of source color and reflection coefficient
		red   += src.red   * refl.red;
		green += src.green * refl.green; 
		blue  += src.blue  * refl.blue;
	}
	void add(Color3& colr)
	{ // add colr to this color
	 red += colr.red ; green += colr.green; blue += colr.blue;}
	void build4tuple(float v[])
	{// load 4-tuple with this color: v[3] = 1 for homogeneous
		v[0] = red; v[1] = green; v[2] = blue; v[3] = 1.0f;
	}
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


// RGBpixmap.h: a class to support working with RGB pixmaps.
#ifndef _RGBPIXMAP
#define _RGBPIXMAP
#include <fstream>

typedef unsigned char uchar;

class mRGB{   // the name RGB is already used by Windows
public: uchar r,g,b;
			mRGB(){r = g = b = 0;}
			mRGB(mRGB& p){r = p.r; g = p.g; b = p.b;}
			mRGB(uchar rr, uchar gg, uchar bb){r = rr; g = gg; b = bb;}
			void set(uchar rr, uchar gg, uchar bb){r = rr; g = gg; b = bb;}
};
//$$$$$$$$$$$$$$$$$ RGBPixmap class $$$$$$$$$$$$$$$
class RGBpixmap{
private: 
	mRGB* pixel; // array of pixels

public:
	int nRows, nCols; // dimensions of the pixmap
	RGBpixmap() {nRows = nCols = 0; pixel = 0;}
	RGBpixmap(int rows, int cols) //constructor
	{
		nRows = rows;
		nCols = cols;
		pixel = new mRGB[rows*cols]; 
	}
	int readBMPFile(string fname); // read BMP file into this pixmap
	void freeIt() // give back memory for this pixmap
	{
		delete []pixel; nRows = nCols = 0;
	}
	//<<<<<<<<<<<<<<<<<< copy >>>>>>>>>>>>>>>>>>>
	void copy(IntPoint from, IntPoint to, int x, int y, int width, int height)
	{ // copy a region of the display back onto the display
		if(nRows == 0 || nCols == 0) return;
		glCopyPixels(x, y, width, height,GL_COLOR);
	}
	//<<<<<<<<<<<<<<<<<<< draw >>>>>>>>>>>>>>>>>
	void draw()
	{ // draw this pixmap at current raster position
		if(nRows == 0 || nCols == 0) return;
		//tell OpenGL: don’t align pixels to 4 byte boundaries in memory
		glPixelStorei(GL_UNPACK_ALIGNMENT,1);
		glDrawPixels(nCols, nRows,GL_RGB, GL_UNSIGNED_BYTE,pixel);
	}
	//<<<<<<<<<<<<<<<<< read >>>>>>>>>>>>>>>>
	int read(int x, int y, int wid, int ht)
	{ // read a rectangle of pixels into this pixmap
		nRows = ht;
		nCols = wid;
		pixel = new mRGB[nRows *nCols]; if(!pixel) return -1;
		//tell OpenGL: don’t align pixels to 4 byte boundaries in memory
		glPixelStorei(GL_PACK_ALIGNMENT,1);
		glReadPixels(x, y, nCols, nRows, GL_RGB,GL_UNSIGNED_BYTE,pixel);
		return 0;
	}
	//<<<<<<<<<<<<<<<<< read from IntRect >>>>>>>>>>>>>>>>
	int read(IntRect r)
	{ // read a rectangle of pixels into this pixmap
		nRows = r.top - r.bott;
		nCols = r.right - r.left;
		pixel = new mRGB[nRows *nCols]; if(!pixel) return -1;
		//tell OpenGL: don’t align pixels to 4 byte boundaries in memory
		glPixelStorei(GL_PACK_ALIGNMENT,1);
		glReadPixels(r.left,r.bott, nCols, nRows, GL_RGB, GL_UNSIGNED_BYTE, pixel);
		return 0;
	}
	//<<<<<<<<<<<<<< setPixel >>>>>>>>>>>>>
	void setPixel(int x, int y, mRGB color)
	{
		if(x>=0 && x <nCols && y >=0 && y < nRows)
			pixel[nCols * y + x] = color;
	}
	//<<<<<<<<<<<<<<<< getPixel >>>>>>>>>>>
	mRGB getPixel(int x, int y)
	{ 		
		mRGB bad(255,255,255);
		assert(x >= 0 && x < nCols);
		assert(y >= 0 && y < nRows);
		return pixel[nCols * y + x];
	}
}; //end of class RGBpixmap
#endif

// RGBpixmap.cpp - routines to read a BMP file
#include "RGBpixmap.h"
typedef unsigned short ushort;
typedef unsigned long ulong;
fstream inf; // global in this file for convenience
//<<<<<<<<<<<<<<<<<<<<< getShort >>>>>>>>>>>>>>>>>>>>
ushort getShort() //helper function
{ //BMP format uses little-endian integer types
  // get a 2-byte integer stored in little-endian form
		char ic;
		ushort ip;
		inf.get(ic); ip = ic;  //first byte is little one 
		inf.get(ic);  ip |= ((ushort)ic << 8); // or in high order byte
		return ip;
}
//<<<<<<<<<<<<<<<<<<<< getLong >>>>>>>>>>>>>>>>>>>
ulong getLong() //helper function
{  //BMP format uses little-endian integer types
   // get a 4-byte integer stored in little-endian form
		ulong ip = 0;
		char ic = 0;
		unsigned char uc = ic;
		inf.get(ic); uc = ic; ip = uc;
		inf.get(ic); uc = ic; ip |=((ulong)uc << 8);
		inf.get(ic); uc = ic; ip |=((ulong)uc << 16);
		inf.get(ic); uc = ic; ip |=((ulong)uc << 24);
		return ip;
	}
//<<<<<<<<<<<<<<<<<< RGBPixmap:: readBmpFile>>>>>>>>>>>>>
int RGBpixmap:: readBMPFile(string fname) 
{  // Read into memory an mRGB image from an uncompressed BMP file.
	// return 0 on failure, 1 on success
	inf.open(fname.c_str(), ios::in|ios::binary); //read binary char's
	if(!inf){ cout << " can't open file: " << fname << endl; return 0;}
	int k, row, col, numPadBytes, nBytesInRow;
	// read the file header information
	char ch1, ch2;
	inf.get(ch1); inf.get(ch2); //type: always 'BM'
	ulong fileSize =      getLong();
	ushort reserved1 =    getShort();    // always 0
	ushort reserved2= 	getShort();     // always 0 
	ulong offBits =		getLong(); // offset to image - unreliable
	ulong headerSize =     getLong();     // always 40
	ulong numCols =		getLong(); // number of columns in image
	ulong numRows = 		getLong(); // number of rows in image
	ushort planes=    	getShort();      // always 1 
	ushort bitsPerPixel=   getShort();    //8 or 24; allow 24 here
	ulong compression =    getLong();      // must be 0 for uncompressed 
	ulong imageSize = 	getLong();       // total bytes in image 
	ulong xPels =    	getLong();    // always 0 
	ulong yPels =    	getLong();    // always 0 
	ulong numLUTentries = getLong();    // 256 for 8 bit, otherwise 0 
	ulong impColors = 	getLong();       // always 0 
	if(bitsPerPixel != 24) 
	{ // error - must be a 24 bit uncompressed image
		cout << "not a 24 bit/pixelimage, or is compressed!\n";
		inf.close(); return 0;
	} 
	//add bytes at end of each row so total # is a multiple of 4
	// round up 3*numCols to next mult. of 4
	nBytesInRow = ((3 * numCols + 3)/4) * 4;
	numPadBytes = nBytesInRow - 3 * numCols; // need this many
	nRows = numRows; // set class's data members
	nCols = numCols;
pixel = new mRGB[nRows * nCols]; //make space for array
	if(!pixel) return 0; // out of memory!
	long count = 0;
	char dum;
	for(row = 0; row < nRows; row++) // read pixel values
	{
		for(col = 0; col < nCols; col++)
		{
			char r,g,b;
			inf.get(b); inf.get(g); inf.get(r); //read bytes
			pixel[count].r = r; //place them in colors
			pixel[count].g = g;
			pixel[count++].b = b;
		}
   		for(k = 0; k < numPadBytes ; k++) //skip pad bytes at row's end
			inf >> dum;
	}
	inf.close(); return 1; // success
}


// Noise class for generating pseudorandom noise fields
//based on noise lattice a la Peachey/Perlin
#include <assert.h>

class Noise{
public:
	Noise()//construct a noise object
	{		
		int i;
		index = new unsigned char[256]; assert(index);
		for(i = 0; i < 256; i++) index[i] = i;//fill array with indices
		for(i = 0; i < 256; i++) // shuffle it
		{
			int which = rand() % 256; // choose random place in array
			unsigned char tmp = index[which]; // swap them
			index[which] = index[i];
			index[i] = tmp;
		}
		noiseTable = new float[256]; assert(noiseTable);
		for(i = 0; i < 256; i++) noiseTable[i] = rand()/32767.99;
	} // end of constructor

	float noise(float scale, Point3& p)
	{ // linearly interpolated lattice noise
		#define lerp(f, A, B) A + f * (B - A)
		float d[2][2][2];
		Point3 pp;
		pp.x = p.x * scale + 10000; //offset avoids negative values
		pp.y = p.y * scale + 10000; 
		pp.z = p.z * scale + 10000;
		long ix = (long)pp.x; long iy = (long)pp.y; long iz = (long)pp.z;
		float tx,ty,tz, x0,x1,x2,x3, y0,y1;
		tx = pp.x - ix; ty = pp.y - iy; tz = pp.z - iz; // fractional parts
		float mtx = 1.0 - tx, mty = 1.0 - ty, mtz = 1.0 - tz;

		for(int k = 0; k <= 1; k++) // get noise at 8 lattice points
		for(int j = 0; j <= 1; j++)
		for(int i = 0; i <= 1; i++)
			d[k][j][i] = latticeNoise(ix + i, iy + j,iz + k);
			
		x0 = lerp(tx, d[0][0][0],d[0][0][1]);
		x1 = lerp(tx, d[0][1][0],d[0][1][1]);
		x2 = lerp(tx, d[1][0][0],d[1][0][1]);
		x3 = lerp(tx, d[1][1][0],d[1][1][1]);
		y0 = lerp(ty, x0, x1);
		y1 = lerp(ty, x2, x3);
		return lerp(tz, y0, y1);
	}
	float turbulence(float s, Point3& p)
	{
		float val = noise(s    , p) / 2 +
					   noise(s * 2, p) / 4 +
					   noise(s * 4, p) / 8 +
					   noise(s * 8, p) / 16;
		return val;
	}
	float marble(float strength,Point3& p)
	{
		float turbul = turbulence(10, p);
		float val = sin(6 * p.z + strength * turbul);
		return mySpline(val);
	}
private:
	float* noiseTable; // array of noise values
	unsigned char * index; //Pseudo random indices
	float mySpline(float x) // used for marble
	{
		if(x <-0.4) return 0.15 +  2.857 * SQR(x + 0.75);
		else if(x < 0.4) return 0.95 - 2.8125 * SQR(x);
		else return 0.26 + 2.666 * SQR(x - 0.7);
	}
	float latticeNoise(int i, int j, int k)
	{ // return PR noise value on an integer lattice
		#define PERM(x) index[(x) & 255]
		#define INDEX(ix, iy, iz) PERM( (ix) + PERM((iy) + PERM(iz)) )
		return noiseTable[INDEX(i,j,k)];
	}
}; // end of Noise class

// uses several of the basic classes developed above
// uses classes: DefUnit, DefUnitStack 
// uses Affine4 and associated classes
// uses Shapes class as well

//@@@@@@@@@@@@@  Scene class @@@@@@@@@@@@@@@@@@@@
class Scene{
public:
	Light *light;  // attach linked list of lights here
	GeomObj * obj;  // attach the object list here
	Color3 background, ambient;
	int maxRecursionDepth;
	RGBpixmap pixmap[8]; //list of attached pixmaps
	float minReflectivity, minTransparency;
	bool isInShadow(Ray& f); // implementation left to the reader
	Scene():light(NULL),obj(NULL),tail(NULL) //default constructor
	{
		currMtrl.setDefault();
		background.set(0,0,0.6f);
		ambient.set(0.1f,0.1f,0.1f);
		minReflectivity = 0.5;
		minTransparency = 0.5;
		maxRecursionDepth = 3;
	} 
	Scene(string fname)  
 { //constructor that also reads in an SDL file
		Scene(); read(fname);
 }
 void freeScene() 
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
	bool read(string fname);
	Color3 shade(Ray& ray); // implementation left to the reader
	GeomObj* getObject();
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
	//<<<<<<<< methods >>>>>>>>>>>
	string nexttoken(void) //########## nexttoken()
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
	float getFloat() //############ getFloat()
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
	bool isidentifier(string keyword) { //######## isidentifier
		string temp = keyword;
		if (!isalpha(temp[0])) return(false);
		for (int count = 1; count < temp.length(); count++) {
			if ((!isalnum(temp[count]))&& (temp[count]!='.')) return(false);
		}
		return(true);
	}
	void cleanUp() //######### cleanUp
	{ // release stuff after parsing file
		affStk.releaseAffines(); 		//delete affine stack
		def_stack->release();
		delete def_stack; // release the DefUnitStack memory
	}
	//+++++++++++++ TokenType +++++++++++++
	enum TokenType {IDENT, LIGHT, ROTATE, TRANSLATE, 
		SCALE, PUSH, POP, IDENTITYAFFINE, 
		GLOBALAMBIENT, BACKGROUND, MINREFLECTIVITY,
		MINTRANSPARENCY, MAXRECURSIONDEPTH, CUBE, SPHERE, TORUS, PLANE,
    SQUARE, CYLINDER, CONE, TAPEREDCYLINDER,TETRAHEDRON, OCTAHEDRON, 
		DODECAHEDRON, ICOSAHEDRON,BUCKYBALL, TEAPOT,
		DIAMOND,UNION,INTERSECTION, DIFFERENCEa, MAKEPIXMAP,
		MESH, DEFAULTMATERIALS, AMBIENT, DIFFUSE,SPECULAR,
    SPECULARFRACTION, SURFACEROUGHNESS,EMISSIVE, SPECULAREXPONENT,
    SPEEDOFLIGHT, TRANSPARENCY,REFLECTIVITY, PARAMETERS, TEXTURE,
    LFTCURLY, RGHTCURLY, DEF, USE, T_NULL, F_EOF, UNKNOWN };
	//<<<<<<<<<<<<<whichtoken >>>>>>>>>>>>>>
	TokenType whichtoken(string keyword) 
	{
		string temp = keyword;
		if ( temp == "light" )            return (LIGHT);
		if ( temp == "rotate" )           return (ROTATE);
		if ( temp == "translate" )        return (TRANSLATE);
		if ( temp == "scale")             return (SCALE);
		if ( temp == "push")        	  return (PUSH);
		if ( temp == "pop"			  return (POP);
		if ( temp == "identityAffine")    return (IDENTITYAFFINE);
		if ( temp == "cube")              return (CUBE);
		if ( temp == "sphere")            return (SPHERE);
		if ( temp == "torus")             return (TORUS);
		if ( temp == "plane")             return (PLANE);
		if ( temp == "square")            return (SQUARE);
		if ( temp == "cylinder")          return (CYLINDER);
		if ( temp == "taperedCylinder")   return (TAPEREDCYLINDER);
		if ( temp == "cone")              return (CONE);
		if ( temp == "tetrahedron")       return (TETRAHEDRON);
		if ( temp == "octahedron")        return (OCTAHEDRON);
		if ( temp == "dodecahedron")      return (DODECAHEDRON);
		if ( temp == "icosahedron")       return (ICOSAHEDRON);
		if ( temp == "buckyball")         return (BUCKYBALL);
		if ( temp == "diamond")           return (DIAMOND);
		if ( temp == "teapot")		   return (TEAPOT);
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
		if ( temp == "texture")		  return (TEXTURE);
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
}; // end of Scene.h

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
	TokenType typ;
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
				currMtrl.textureType = getFloat();}	
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
		  int which = getFloat();// index of this pixmap in pixmap array
		  if(which < 0 || which > 7){cout << "\nbad index of RGBpixmap!\n";}
		  string fname = nexttoken(); // get file name for mesh	  
		  cout << "I got fname = " << fname << endl;
		  if(!pixmap[which].readBMPFile(fname))
		  {// read BMP file into this pixmap
				cout << " \ncan't read that RGBpixmap file!\n";
				return NULL;
		  }
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
	DefUnitStack() {
		stack = NULL;
	}
	void push(string n, string s) { 
		D4S *temp_d4s = new D4S;
		temp_d4s->current = new DefUnit(n, s);
		temp_d4s->next = stack;
		stack = temp_d4s;
	}
	void print() {
		D4S *temp = stack;
		string t;
		while (temp) {
			cout << temp->current->name << ":" ;
			cout << temp->current->stuff << endl;
			temp = temp->next;
		}
	}
	int search(string s) {
		D4S *temp = stack;
		while (temp) {
			if ( temp->current->name == s ) {
				return(1);
			}
			temp = temp->next;
		}
		return(0);
	}
	string contents(string s) {
		D4S *temp = stack;
		while (temp) {
			if (temp->current->name == s ) {
				return(temp->current->stuff);
			}
			temp = temp->next;
		}
		return(NULL);
	}
	void release()
	{
		while(stack)
		{
			D4S* tmp = stack; // grab it
			//cerr << "releasing def_stack item: "<< tmp->current->name<< endl;
			stack = stack->next; // advance p
			delete tmp->current; // release 2 strings
			delete tmp; // release node
		}
		stack = NULL;
	}
private:
	struct D4S {
		DefUnit *current;
		struct D4S *next;
	} d4s;
	D4S *stack;
}; // end of DefUnitStack class

// @@@@@@@@@@@@@@@@@@@@@ Affine4 class @@@@@@@@@@@@@@@@
class Affine4 {// manages homogeneous affine transformations
// including inverse transformations
// and a stack to put them on
// used by Scene class to read SDL files
public:
	float m[16]; // hold a 4 by 4 matrix
	Affine4(){ // make identity transform
		m[0] = m[5]  = m[10] = m[15] = 1.0;
		m[1] = m[2]  = m[3]  = m[4]  = 0.0;
		m[6] = m[7]  = m[8]  = m[9]  = 0.0;
		m[11]= m[12] = m[13] = m[14] = 0.0;
	}
	void setIdentityMatrix(){ // make identity transform
		m[0] = m[5]  = m[10] = m[15] = 1.0;
		m[1] = m[2]  = m[3]  = m[4]  = 0.0;
		m[6] = m[7]  = m[8]  = m[9]  = 0.0;
		m[11]= m[12] = m[13] = m[14] = 0.0;
	}
	void set(Affine4 a)// set this matrix to a
	{
		for(int i = 0; i < 16; i++)
			m[i]=a.m[i];
	}
	//<<<<<<<<<<<< postMult >>>>>>>>>>>
	void postMult(Affine4 n){// postmultiplies this with n
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
	void dup()
	{
		AffineNode* tmp = new AffineNode;			
		tmp->affn = new Affine4(*(tos->affn));
		tmp->invAffn = new Affine4(*(tos->invAffn));
		tmp->next = tos;
		tos = tmp;
	}
	void setIdentity() // make top item the identity matrix
	{ 
		assert(tos != NULL);
		tos->affn->setIdentityMatrix();
		tos->invAffn->setIdentityMatrix();
	}
	void popAndDrop()
	{
		if(tos == NULL) return; // do nothing
		AffineNode *tmp = tos;
		tos = tos->next;
		delete tmp; // should call destructor, which deletes trices
	}	
	void releaseAffines()
	{ // pop and drop all remaining items 
	while(tos) popAndDrop();
	}
	void rotate(float angle, Vector3 u)
	{
		Affine4 rm; // make identity matrix
		Affine4 invRm;
		u.normalize(); // make the rotation axis unit length
		float ang = angle * 3.14159265/ 180; // deg to  
		float c = cos(ang), s = sin(ang);
		float mc = 1.0 - c;
	 	//fill the 3x3 upper left matrix - Chap.5 p. 29
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
	void scale(float sx, float sy, float sz)
	{ // post-multiply top item by scaling
		#define sEps 0.00001
		Affine4 scl;// make an identity
		Affine4 invScl;
		scl.m[0]  = sx; 
		scl.m[5]  = sy; 
		scl.m[10] = sz;// adjust it to a scaling matrix
		if(fabs(sx) < sEps || fabs(sy) < sEps || fabs(sz) < 
		{
			cerr << "degenerate scaling transformation!\n"; );
		}
		invScl.m[0]  = 1/sx; invScl.m[5]  = 1/sy; m[10] = 1/sz;
		tos->affn->postMult(scl); //
		tos->invAffn->preMult(invScl);
	}		
	void translate(Vector3 d)
	{
		Affine4 tr; // make identity matrix
		Affine4 invTr;
		tr.m[12] = d.x; tr.m[13] = d.y;	tr.m[14] = d.z;
		invTr.m[12] = -d.x;	invTr.m[13] = -d.y; [14] = -d.z;
		tos->affn->postMult(tr);
		tos->invAffn->preMult(invTr);
	}
}; // end of AffineStack class



//@@@@@@@@@@@@@@@@@@@ PointCluster class @@@@@@@@@@@@@@@@@@
class PointCluster{
public: // holds array of points for the bounding hull of a shape
	int num;
	Point3* pt;
	PointCluster() {num = 0; pt = NULL;}
	PointCluster(int n)// make a cluster of n points
	{
		pt = new Point3[n]; assert(pt);
		num = n;
  }
};
//@@@@@@@@@@@@@@@@@@@@@@@@@@ SphereInfo @@@@@@@@@@@@@@@@@@@@@
class SphereInfo{// holds the center and radius of a sphere
public:
	Point3 center;
	float radSq;
	void set(float x, float y, float z, float rsq)
	{
		center.set(x,y,z);
		radSq = rsq;
	}
};
//@@@@@@@@@@@@@@@@@@@@@@@@ Cuboid @@@@@@@@@@@@@@@@@@@@@@@@
class Cuboid{ // holds six border values of a cuboid
public:
	float left, top, right, bott, front, back;
	void set(float l, float t, float r, float b, float f, float bk)
	{
		left = l; top = t; right = r; bott = b; front = f; back = bk;}
	void set(Cuboid& c)
	{
		left = c.left;top = c.top; right = c.right; bott = c.bott;
		front = c.front; back = c.back;
	}
	void print(){
		cout << "\n(l,t,r,b,f,bk) = (" << left << "," << top << "," << right << "," << bott << "," << front << "," << back << ")";
	}
};

//@@@@@@@@@@@@@@@@@@@@ Ray @@@@@@@@@@@@@@@@@@@@@@@@@
class Ray{
public:
	Point3 start;
	Vector3 dir;
	int recurseLevel;
	int row, col; // for screen extents
	int numInside; // number of objects on list
	GeomObj* inside[10]; // array of object pointers

	Ray(){start.set(0,0,0); dir.set(0,0,0); numInside = 0;}
	Ray(Point3 origin); //constructor: set start point of ray
	Ray(Point3& origin, Vector3& direction)
		{ start.set(origin); dir.set(direction); numInside = 0;} 
	void setStart(Point3& p){start.set(p);}
	void setDir(float x, float y, float z)
 {dir.x = x; dir.y = y; dir.z = z;} 
	void setRayDirection(Light *L); //for shadow feelers
	void setRayDirection(Vector3& dir); //for spawned rays
	int isInShadow();
	void makeGenericRay(GeomObj* p, Ray& gr);
};

//@@@@@@@@@@@@@@@@@@ HitInfo @@@@@@@@@@@@@@@@@@@@
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
	}	void set(HitInfo& h)
	{
		hitTime = h.hitTime; hitObject = h.hitObject;	
		surface = h.surface; isEntering = h.isEntering;
		hitPoint.set(h.hitPoint); hitNormal.set(h.hitNormal);
	}
};

//@@@@@@@@@@@@@@@@@ Intersection @@@@@@@@@@@@@@@@@@@@@
class Intersection{ // hold the hit list 
 public:
	#define maxNumHits 8
	int numHits;			 // the number of hits
	HitInfo hit[maxNumHits];	 // list of hits;
	Intersection(){numHits = 0;}  // default constructor
	void set(Intersection& intr)
	{ // copy intersection info
		numHits = intr.numHits;
		for(int i = 0; i < maxNumHits; i++)
			hit[i].set(intr.hit[i]);
	}
};

//Shapes class and Supporting classes
//Shapes.h 
// GeomObj, Boolean, Shape, Cube, Sphere, etc.
// uses pixmap class and basic classes above

#define PI 3.14159265
#define TWOPI (3.14159265 * 2)
#define degPerRad (180/3.14159265)
#define Near(a,v) fabs((a)-(v))<0.0001

//@@@@@@@@@@@@@@@@@ Material class @@@@@@@@@@@@@@
class Material{
public:
	Color3 ambient, diffuse, specular, emissive;
	int numParams; // for textures
	float params[10]; // for textures
	int textureType;  // 0 for none, neg for solids, pos for images
	float specularExponent, reflectivity, transparency, speedOfLight;
	float specularFraction, surfaceRoughness;	
	void setDefault(){
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
	void set(Material& m)
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
}; // end of Material

//@@@@@@@@@@@@@@@@@@@@@ GeomObj class @@@@@@@@@@@@@@@@
class GeomObj{
public: 
		IntRect scrnExtnt;
		Cuboid genBoxExtent,worldBoxExtent;
		SphereInfo genSphereExtent,worldSphereExtent;
		GeomObj * next;
		GeomObj(): next(NULL){}
		virtual bool hit(Ray &r, Intersection &inter){return false;}
		virtual void loadStuff(){}
		virtual void drawOpenGL(){}
		virtual void tellMaterialsGL(){}
		virtual void makeExtentPoints(PointCluster& clust)
		{cout << "in makeExtentPoints for GeomObj";}
		virtual Point2 texturePoint(Point3 p){Point2 pt(0,0); return pt;}
		virtual Color3 texture(HitInfo& h){Color3 c(0,0,0); return c;}
};

//@@@@@@@@@@@@@@@@@@@ Boolean @@@@@@@@@@@@@@@@@@
class Boolean: public GeomObj{
public:
	GeomObj *left, *right;
	Boolean():left(NULL),right(NULL){}
	virtual bool hit(Ray &r, Intersection &inter){return false;}
	virtual void makeExtentPoints(PointCluster& clust)
  {cout << "in makeExtentPoints for Boolean";}
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
	virtual void makeExtentPoints(PointCluster& clust)
	{ // cluster is union of lft and rt clusters
		PointCluster lftClust, rtClust;
		left->makeExtentPoints(lftClust);
		right->makeExtentPoints(rtClust);
		//now combine them
		int i,j;
		clust.num = lftClust.num + rtClust.num;
		clust.pt = new Point3[clust.num];
		assert(clust.pt);
		for(i = 0; i < lftClust.num; i++)
			clust.pt[i] = lftClust.pt[i];
		for(j = 0; j < rtClust.num; j++)
			clust.pt[j + i] = rtClust.pt[j];
	}
	virtual bool hit(Ray &r, Intersection &inter);
};
//@@@@@@@@@@@@@@@@@@@@ IntersectionBool @@@@@@@@@@@@@@@@
class IntersectionBool : public Boolean{
public:
	IntersectionBool(){Boolean();}
	virtual void makeExtentPoints(PointCluster& clust)
	{ //cluster is just left cluster	
		left->makeExtentPoints(clust);
	}
	virtual bool hit(Ray &r, Intersection &inter);
};
//@@@@@@@@@@@@@@@@@@@@ DifferenceBool @@@@@@@@@@@@@@@@
class DifferenceBool : public Boolean{
public:
	DifferenceBool(){Boolean();} 
	virtual void makeExtentPoints(PointCluster& clust)
	{ // cluster is just left cluster
			left->makeExtentPoints(clust);
	}
	virtual bool hit(Ray &r, Intersection &inter);
};
//@@@@@@@@@@@@@@@@@ Shape @@@@@@@@@@@@@@@@@@@@@
class Shape: public GeomObj{
public:
	Material mtrl;
	Affine4 transf,invTransf;
	virtual bool hit(Ray &r, Intersection &inter){return false;}
	//virtual Color3 texture(HitInfo& h, int whichTexture);
	Shape(){mtrl.textureType = 0; mtrl.numParams = 0;}
	void setMaterial(Material& mt){mtrl.set(mt);}
	void tellMaterialsGL()
	{
		float amb[4],diff[4],spec[4], emiss[4];
		float zero[] = {0,0,0,1};
		mtrl.ambient.build4tuple(amb); // fill the array
		mtrl.diffuse.build4tuple(diff);
		mtrl.specular.build4tuple(spec);
		mtrl.emissive.build4tuple(emiss);
		glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_AMBIENT,amb);
		glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_DIFFUSE,diff);
		if(specularOn)
			glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_SPECULAR,spec);
		else 
			glMaterialfv(GL_FRONT,GL_SPECULAR,zero);
		glMaterialfv(GL_FRONT/*_AND_BACK*/,GL_EMISSION,emiss);
		glMaterialf (GL_FRONT/*_AND_BACK*/,GL_SHININESS,mtrl.specularExponent);
	}
	virtual void drawOpenGL(){}
	virtual Point2 texturePoint(Point3 p){Point2 pt(0,0); return pt;}
	virtual void makeExtentPoints(PointCluster& clust){}		
}; //end: Shape class

 
//@$@$@$@$@$@$@$@$@ Cube class $@$@$@$@$@$@$@$@$@$@
class Cube : public Shape{
public:	
	Cube(){}
	void drawOpenGL()
	{
		tellMaterialsGL(); glPushMatrix();
		glMultMatrixf(transf.m); //load affine
		glEnable(GL_NORMALIZE);
		if(doEdges) glutWireCube(2.0);
		if(doFaces) glutSolidCube(2.0);	// a cuber with vertices -1 to +1
		glPopMatrix();
	}
	bool hit(Ray &r, Intersection &inter); //implementation left to the reader
	Point2 texturePoint(Point3 p); // ditto
	void makeExtentPoints(PointCluster& clust); //ditto
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
	bool hit(Ray& r, Intersection& inter);
	Point2 texturePoint(Point3 p);
	void makeExtentPoints(PointCluster& clust);
};
//@$@$@$@$@$@$@$@$@$@ TaperedCylinder class @$@$@$@$@$@$@$@$@$
class TaperedCylinder : public Shape{
public:
		float smallRadius;
		TaperedCylinder(){}
		bool hit(Ray &r, Intersection &inter);		
		void drawOpenGL();
		Point2 texturePoint(Point3 p);
		void makeExtentPoints(PointCluster& clust);
};
//@$@$@$@$@$@$@$@$@$@ Square class @$@$@$@$@$@$@$@$@$
class Square : public Shape{
public:
		Square(){}
		bool hit(Ray &r, Intersection &inter);
		void drawOpenGL();
		Point2 texturePoint(Point3 p);
		void makeExtentPoints(PointCluster& clust);
};
//@$@$@$@$@$@$@$@$@$@ Plane class @$@$@$@$@$@$@$@$@$
class Plane : public Shape{
public:
		Plane() {}
		bool hit(Ray &r, Intersection &inter);
		void drawOpenGL();
		Point2 texturePoint(Point3 p);
		void makeExtentPoints(PointCluster& clust);
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
		void freeMesh()
		{ // free up memory used by this mesh.
			delete [] pt; // release whole vertex list
			delete [] norm;
			for(int f = 0; f < numFaces; f++)
				delete[] face[f].vert; // delete the vert[] array of this face
			delete [] face;
		}
		int isEmpty() 
		{return (numVerts == 0) || (numFaces == 0) || (numNorms == 0);}
		void makeEmpty() { numVerts = numFaces = numNorms = 0;}
		Mesh(){
			numVerts = numFaces = numNorms = 0; 
			pt = NULL; norm  =  NULL; face = NULL;
			lastVertUsed = lastNormUsed = lastFaceUsed = -1;
		}
		virtual void drawOpenGL()
		{ 
			tellMaterialsGL(); 	glPushMatrix();
			glMultMatrixf(transf.m); 
			if(doFaces) drawMesh();	
			if(doEdges) drawEdges();
			glPopMatrix();
		} 
		Mesh(string fname){ // read this file to build mesh
			numVerts = numFaces = numNorms = 0; 
			pt = NULL; norm  =  NULL; face = NULL;
			lastVertUsed = lastNormUsed = lastFaceUsed = -1;
			readMesh(fname);
		}
		Vector3 newell4(int indx[])
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
public:
		string meshFileName; // holds file name for this Mesh
		void print(){cout << "\nMesh ";mtrl.print();}
		bool hit(Ray &r, Intersection &inter);
		Point2 texturePoint(Point3 p);
		void makeExtentPoints(PointCluster& clust);
};
//@$@$@$@$@$@$@$@$@$@ Torus class @$@$@$@$@$@$@$@$@$
class Torus : public Shape{
public:
		bool hit(Ray &r, Intersection &inter){return false;}
		void drawOpenGL(){ 
			tellMaterialsGL(); 	glPushMatrix();
			glMultMatrixf(transf.m); 
			if(doFaces) glutSolidTorus(0.2,1.0,10,12);	
			if(doEdges) glutWireTorus(0.2,1.0,10,12);	
			glPopMatrix();}
};
//@$@$@$@$@$@$@$@$@$@ Teapot class @$@$@$@$@$@$@$@$@$
class Teapot : public Shape{
public:
		bool hit(Ray &r, Intersection &inter){return false;}
		void drawOpenGL(){ tellMaterialsGL(); glPushMatrix();
		glMultMatrixf(transf.m); 
		glutSolidTeapot(1.0); glPopMatrix();}
};

//<<<<<<<<<<<<<<<<<< hit() for Sphere >>>>>>>>>>>>>>>>>>>>>>>
	bool Sphere:: hit(Ray &r, Intersection& inter)
	{  // intersect the ray with the generic sphere		
		if(doScreenExtent && r.recurseLevel == 0)
			if( r.col < scrnExtnt.left ||
				r.col > scrnExtnt.right||
				r.row < scrnExtnt.bott ||
				r.row > scrnExtnt.top )return false; //misses screen extent	
			Ray genRay;
			xfrmRay(genRay,invTransf,r);			
			double A, B, C, discrim, disc_root, t1, t2;
			A = dot3D(genRay.dir, genRay.dir); 
			assert(A > 0); // fails if ray is sick 
			B = dot3D(genRay.start, genRay.dir);
			C = dot3D(genRay.start, genRay.start) - 1.0;
			discrim = B * B - A * C;
			if(discrim < 0.0)return false;   // ray missed: numHits = 0 
			disc_root = sqrt(discrim);		
			inter.numHits = 0;	// no hits yet	
			t1 = (-B - disc_root)/A;         // earlier hit 
			if(t1 > 0.00001) // a pos hit time
			{
				inter.hit[0].hitTime = t1;
				Point3 P(rayPos(genRay, t1));// position of hit spot
				inter.hit[0].hitPoint.set(P);
				inter.hit[0].hitObject = this;
				inter.hit[0].isEntering = 1;
				inter.hit[0].surface = 0;
				inter.hit[0].hitNormal.set(P); // vector is same as hit spot
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
				inter.hit[inter.numHits].hitNormal.set(P); //same as hit spot
				inter.numHits++;			// got another
			}
			return inter.numHits > 0;
	}// end of hit() for Sphere

//<<<<<<<<<<<<<<<<<<<<<<<<<<<< readMesh >>>>>>>>>>>>>>>>>>>>>>>>
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
//<<<<<<<<<<<<<<<<<<<<<<<<<<<< write mesh>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void Mesh:: writeMesh(char * fname)
{ // write this mesh object into a new Chapter 6 format file.
	if(numVerts == 0 || numNorms   == 0 || numFaces == 0) return; //empty
	fstream outStream(fname, ios ::out); // open the output stream
	if(outStream.fail()) {cout << "can't make new file: " << fname << endl; return;}
	outStream << numVerts << " " << numNorms << " " << numFaces << "\n";
	// write the vertex and vertex normal list
	for(int i = 0; i < numVerts; i++)
		outStream << pt[i].x   << " " << pt[i].y   << " " << pt[i].z << "\n";
	for(int ii = 0; ii < numNorms; ii++)
		outStream  << norm[ii].x << " " << norm[ii].y << " " << norm[ii].z << "\n";
	// write the face data
	for(int f = 0; f < numFaces; f++)
	{
		int n = face[f].nVerts;
		outStream << n << "\n";
		for(int v = 0; v < n; v++)// write vertex indices for this face
			outStream << face[f].vert[v].vertIndex << " ";	outStream << "\n";
		for(int k = 0; k < n; k++)	// write normal indices for this face 
			outStream << face[f].vert[k].normIndex << " "; outStream << "\n";
	}
	outStream.close();
}
