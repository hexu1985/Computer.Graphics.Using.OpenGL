// this includes code from Figs. 3.25 3.26 3.27 3.28 
// and page 101

#include<gl/glut.h>
#include<math.h>
#include<iostream.h>

//<<Support Classes for Canvas>>>
class Point2   //single point w/ floating point coordinates
{
public:
	Point2() {x = y = 0.0f;} //constructor 1
	Point2(float xx, float yy) {x=xx; y=yy;} //constructor 2
	void set(float xx, float yy) {x=xx; y=yy;}
	float getX() {return x;}
	float getY() {return y;}
	void draw(void)
	{
		glBegin(GL_POINTS); //draw this point
		glVertex2f((GLfloat)x, (GLfloat)y);
		glEnd();
	}
private:
	float x, y;
};

class IntRect  //aligned rectangle with integer coordinates, used for viewport
{
public:
	IntRect() {l = 0; r=100; b=0; t=100;} //constructors
	IntRect(int left, int right, int bottom, int top)
		{l = left; r=right; b=bottom; t=top;}
	void set(int left, int right, int bottom, int top)
	{
		l=left; r=right; b=bottom; t=top;
	}
	void draw(void); //draw this rectangle using OpenGL
	int getL(void)
	{
		return l;
	}
	int getR(void)
	{
		return r;
	}
	int getT(void)
	{
		return t;
	}
	int getB(void)
	{
		return b;
	}

private:
	int l, r, b, t;
};

class RealRect   //simlar to IntRect but w/ floating points & used for world window
{
public:
	RealRect() {l = 0; r=100; b=0; t=100;} //constructors
	RealRect(float left, float right, float bottom, float top)
		{l = left; r=right; b=bottom; t=top;}
	void set(float left, float right, float bottom, float top)
	{
		l=left; r=right; b=bottom; t=top;
	}
	float getL(void)
	{
		return l;
	}
	float getR(void)
	{
		return r;
	}
	float getT(void)
	{
		return t;
	}
	float getB(void)
	{
		return b;
	}
	void draw(void); //draw this rectangle using OpenGL
private:
	float l, r, b, t;
};


//<<End Support Classes>>>



class Canvas
{
public:
	Canvas(int width, int height, char* windowTitle); //constructor
        void setWindow(float l, float r, float b, float t);
        void setViewport(int l, int r, int b, int t);
        IntRect getViewport(void); //divulge the viewport data
        RealRect getWindow(void); // divulge the window data
        float getWindowAspectRatio(void);
        void clearScreen();
        void setBackgroundColor(float r, float g, float b);
        void setColor(float r, float g, float b);
        void lineTo(float x, float y);
        void lineTo(Point2 p);
        void moveTo(float x, float y);
        void moveTo(Point2 p);
		void moveRel(float dx, float dy);

private:
     Point2 CP;         //current position in the world
     IntRect viewport;  //the current window
     RealRect window;   //the current viewport

} ;

//<<moveTo>>
//changes current point
void Canvas:: moveTo(float x, float y)
{
	CP.set(x, y);
}

//<<lineTo>>
//draws a line from current point to new point
void Canvas:: lineTo(float x, float y)
{
	glBegin(GL_LINES);
		glVertex2f((GLfloat) CP.getX(), (GLfloat) CP.getY());
		glVertex2f((GLfloat) x, (GLfloat) y); //draw the line
	glEnd();
	CP.set (x, y); //update current point to new point
	glFlush();
}

//<<setWindow>>
void Canvas:: setWindow (float l, float r, float b, float t)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D ((GLdouble)1, (GLdouble)r, (GLdouble)b, (GLdouble)t);
	window.set (l, r, b, t);
}

//<<setViewport>>
void Canvas:: setViewport (int l, int r, int b, int t)
{
	glViewport(l, b, r-l, t-b);
	viewport.set (l, r, b, t);
}

IntRect Canvas:: getViewport(void)
{
	return viewport;
}

RealRect Canvas:: getWindow(void)
{
	return window;
}

void Canvas:: clearScreen(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Canvas:: setBackgroundColor(float r, float g, float b)
{
	glClearColor(r, g, b, 0.0);   //4th variable level of transparency, may need to change 
}

void Canvas:: setColor(float r, float g, float b)
{
	glColor3f(r, g, b);
}

void Canvas:: lineTo(Point2 p)
{
	glBegin (GL_LINES);
		glVertex2f((GLfloat) CP.getX(), (GLfloat) CP.getY());
		glVertex2f((GLfloat) p.getX(), (GLfloat) p.getY());
	glEnd();
	CP.set(p.getX(), p.getY());
	glFlush();
}

Canvas:: Canvas(int width, int height, char* windowTitle)
{
	char* argv[1];  //dummy argument list for glutinit()
	char dummyString[8];
	argv[0] = dummyString;  //hook up the pointer
	int argc = 1;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition (20,20);
	glutCreateWindow (windowTitle);
	setWindow(0, (float)width, 0, (float)height); // default world window
	setViewport (0, width, 0, height); //default viewport
	CP.set(0.0f, 0.0f); //initialize the cp to (0,0)
}

void Canvas:: moveTo(Point2 p)  //moves current point CP to point p object 
{
	float x1, y1;
	x1 = p.getX();
	y1 = p.getY();
	CP.set(x1, y1);
}

float Canvas:: getWindowAspectRatio(void)  //calculates aspect ratio of world window
{
	float width, height, aspectRatio;
	width = window.getR() - window.getL();
	height = window.getT() - window.getB();
	aspectRatio = width/height;
	return aspectRatio;
}

void Canvas:: moveRel(float dx, float dy)
{
	CP.set(CP.getX() + dx, CP.getY() + dy);
}

Canvas cvs(640, 480, "try out Canvas"); //global canvas object

void ngon(int n, float cx, float cy, float radius, float rotAngle)
{
	//assumes global Canvas object, cvs
	if(n<3) return ; //bad number of sides
	double angle = rotAngle * 3.14159265 / 180; //initial angle
	double angleInc = 2 * 3.14159265 / n;  //angle increment
	cvs.moveTo(radius * cos(angle) + cx, radius * sin(angle) + cy);
	for(int k=0; k<n; k++) //repeat n times
	{
		angle += angleInc;
		cvs.lineTo(radius * cos(angle) + cx, radius + sin(angle) + cy);
	}
}



void drawCircle(Point2 center, float radius)
{	
	
	glColor3f(1.0, 0.0, 0.0);
	
	const int numVerts = 40;
	ngon(numVerts, center.getX(), center.getY(), radius, 0);
	glFlush();
}

void display(void)
{
	void moveBall(Point2 p);
	cvs.clearScreen();
	
	Point2 point(5.0, -6.0);
	moveBall(point);
}

void moveBall(Point2 p)
{
	int n = 1;
	RealRect win(-8.0, 8.0, -10.0, 10.0); 
	win = cvs.getWindow(); //sets current window to realRect win in order to get coordinates
	float x1 = 0.025;      //initial variance of x & y
	float y1 = 0.025;
	for(int t = 0; ; t++) //infinate loop, hopefully
	{
		
		if(p.getX() >= win.getR()-0.25) //checks to see if p.x isn't outside of the right wall
		{
			x1 = -0.005; //if so sets x neg direction
			y1 = 0.005;	 //y pos direction
		}
		if(p.getX() <= 1.0+0.25 /*p.getX() <= win.getL()+0.25*/) //checks p.x isn't outside of the left wall
		{
			x1 = 0.005;
			y1 = -0.025;
		}
		if(p.getY() >= 9.0 - 0.25 /*win.getT()-0.25*/) //checks p.y isn't outside of the top wall
		{
			x1 = -0.025;
			y1 = -0.005;
		}
		if(p.getY() <= -9.0+0.25/*win.getB()+.25*/)  //checks p.y isn't outside of the bottom wall
		{
			x1 = 0.005;
			y1 = 0.025;
		}
		cvs.clearScreen();			//clears the screen
		p.set(p.getX() + x1, p.getY() + y1);  //adds or sub. p.x & p.y coordinates
		drawCircle(p, 0.25);	//draws circle at p.x, p.y w/ radius .25
		glutSwapBuffers();  //swap buffers
		cout<<p.getX()<<", "<<p.getY()<<"\n";
	}	
}


// example of using canvas class
// bounces a ball inside a 2D window

void main(void)
{
	cvs.setWindow(-8.0, 8.0, -10.0, 10.0);
	cvs.setViewport(0, 640, 0, 480); 
	cvs.setBackgroundColor(1.0, 1.0, 1.0);
	cvs.setColor(0.0, 0.0, 0.0);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutDisplayFunc(display);
	glutMainLoop();
	
	
}

