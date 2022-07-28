#include <fstream>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

const char* g_file_name = "./dino.dat";

void drawPolyLineFile(const char *fileName)
{
    std::fstream inStream;
    inStream.open(fileName, std::ios::in);    // open the file
    if(inStream.fail())
        return;
    glClear(GL_COLOR_BUFFER_BIT);      // clear the screen 
    GLint numpolys, numLines, x ,y;
    inStream >> numpolys;                   // read the number of polylines
    for(int j = 0; j < numpolys; j++)  // read each polyline
    {
        inStream >> numLines;
        glBegin(GL_LINE_STRIP);         // draw the next polyline
        for (int i = 0; i < numLines; i++)
        {
            inStream >> x >> y;        // read the next x, y pair
            glVertex2i(x, y);
        }
        glEnd();
    }
    glFlush();
    inStream.close();
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
    drawPolyLineFile(g_file_name);
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
