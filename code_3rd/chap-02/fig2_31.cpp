//#include <windows.h>   // use as needed for your system
#include <vector>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

struct GLintPoint {
    GLint x;
    GLint y;
};

GLintPoint CP;         // global current position

//<<<<<<<<<<<<< moveto >>>>>>>>>>>>>>
void moveto(GLint x, GLint y)
{
	CP.x = x; CP.y = y; // update the CP 
}

//<<<<<<<<<<<< lineTo >>>>>>>>>>>>>>>>>
void lineto(GLint x, GLint y)
{
	glBegin(GL_LINES);  // draw the line 
		glVertex2i(CP.x, CP.y);
		glVertex2i(x, y);
	glEnd();
	glFlush();
	CP.x = x; CP.y = y; // update the CP 
}

void drawPolyLine(const std::vector<GLintPoint>& poly, bool closed)
{
    if (poly.size() < 2)
        return;

    moveto(poly[0].x, poly[0].y);
    for (int i = 1; i < poly.size(); i++)
        lineto(poly[i].x, poly[i].y);

    if (closed) 
       lineto(poly[0].x, poly[0].y); 
}

void parameterizedHouse(GLintPoint peak, GLint width, GLint height)
 // the top of house is at the peak; the size of house is given
 //  by height and width
{
    // draw shell of house 
    std::vector<GLintPoint> shell_of_house = {
        {peak.x,             peak.y}, 
        {peak.x + width / 2, peak.y - 3 * height / 8},
        {peak.x + width / 2, peak.y -     height},
        {peak.x - width / 2, peak.y -     height},
        {peak.x - width / 2, peak.y - 3 * height / 8}
    };
    drawPolyLine(shell_of_house, true);

    // draw the chimney 
    std::vector<GLintPoint> chimney = {
		{peak.x - width / 3, peak.y - height / 4},	
		{peak.x - width / 3, peak.y},
		{peak.x - width / 6, peak.y},
		{peak.x - width / 6, peak.y - height / 8}
    };
    drawPolyLine(chimney, false);

    // draw the door 
    std::vector<GLintPoint> door = {
		{peak.x - width / 3, peak.y -     height},	
		{peak.x - width / 3, peak.y - 9 * height / 16},
		{peak.x - width / 12, peak.y - 9 * height / 16},
		{peak.x - width / 12, peak.y -     height}
    };
    drawPolyLine(door, false);

    // draw the window 
    std::vector<GLintPoint> window = {
		{peak.x + width / 12, peak.y - 5 * height / 8},    
		{peak.x + width / 12, peak.y - 7 * height / 16},	
		{peak.x + width / 3, peak.y - 7 * height / 16},
		{peak.x + width / 3, peak.y - 5 * height / 8}
    };
    drawPolyLine(window, true);
}

//<<<<<<<<<<<<<<<<<<<<<<< myInit >>>>>>>>>>>>>>>>>>>>
void myInit(void)
{
    glClearColor(1.0,1.0,1.0,0.0);       // set white background color
    glColor3f(0.0f, 0.0f, 0.0f);          // set the drawing color 
    glPointSize(4.0);               // is 4 by 4 pixels
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);     // clear the screen 
    parameterizedHouse(GLintPoint{70,120}, 60, 80);
    parameterizedHouse(GLintPoint{90,200}, 30, 40);
    parameterizedHouse(GLintPoint{200,200}, 90, 60);
    parameterizedHouse(GLintPoint{300,200}, -90, -60);
    parameterizedHouse(GLintPoint{70,250}, 60, -80);
    parameterizedHouse(GLintPoint{400,200}, 60, 120);
    parameterizedHouse(GLintPoint{400,300}, -90, 60);
    glFlush();
}

//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char** argv)
{
    glutInit(&argc, argv);          // initialize the toolkit
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
    glutInitWindowSize(640,480);     // set window size
    glutInitWindowPosition(100, 150); // set window position on screen
    glutCreateWindow("my first attempt"); // open the screen window
    glutDisplayFunc(myDisplay);     // register redraw function
    myInit();                   
    glutMainLoop();              // go into a perpetual loop

    return 0;
}
