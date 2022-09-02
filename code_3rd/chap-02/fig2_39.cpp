#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

const int screenWidth = 640;       // width of screen window in pixels 
const int screenHeight = 480;       // height of screen window in pixels

struct GLintPoint {
    GLint x;
    GLint y;
};

int random(int n)
{
    return rand()%n;
}

void Sierpinski(GLintPoint T[]) 
{
    glClear(GL_COLOR_BUFFER_BIT);     // clear the screen 
    int index = random(3);         // 0, 1, or 2 equally likely 
    GLintPoint point = T[index];      // initial point 
    glBegin(GL_POINTS);
        glVertex2i(point.x, point.y);     // draw initial point 
        for(int i = 0; i < 55000; i++)  // draw 55000 dots
        {
             index = random(3);
             point.x = (point.x + T[index].x) / 2;
             point.y = (point.y + T[index].y) / 2;
             glVertex2i(point.x,point.y);  
        } 
    glEnd();    
    glFlush();     
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
    glFlush();     
}

//<<<<<<<<<<<<<<<<<<<<<<<< myMouse >>>>>>>>>>>>>>>>>
void myMouse(int button, int state, int x, int y)
{
    static GLintPoint corners[3];
    static int numCorners = 0;
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {        
        corners[numCorners].x = x;
        corners[numCorners].y = screenHeight-y;
        if (++numCorners == 3)
        {
            Sierpinski(corners);
            numCorners = 0;
        }
    }
    glFlush();
}

//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char** argv)
{
    glutInit(&argc, argv);          // initialize the toolkit
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // set display mode
    glutInitWindowSize(screenWidth, screenHeight);     // set window size
    glutInitWindowPosition(100, 150); // set window position on screen
    glutCreateWindow("my first attempt"); // open the screen window
    glutDisplayFunc(myDisplay);     // register redraw function
    glutMouseFunc(myMouse);
    myInit();                   
    glutMainLoop();              // go into a perpetual loop

    return 0;
}
