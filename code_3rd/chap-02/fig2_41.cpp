// 允许用户画橡皮矩形程序: 长方形大小随鼠标变化
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

struct GLintPoint {
    GLint x;
    GLint y;
};

GLintPoint corner[2];
bool selected = false;
int screenWidth = 640, screenHeight = 480;

//<<<<<<<<<<<<<<<<<<<<<<<< myDisplay >>>>>>>>>>>>>>>>>
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);     // clear the screen 
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);
    if (selected) {
        glBegin(GL_QUADS);
        glVertex2i(corner[0].x, corner[0].y);    // 画一个长方形
        glVertex2i(corner[0].x, corner[1].y);
        glVertex2i(corner[1].x, corner[1].y);
        glVertex2i(corner[1].x, corner[0].y);
        glEnd();
    }
    glutSwapBuffers();
}

//<<<<<<<<<<<<<<<<<<<<<<<< myMouse >>>>>>>>>>>>>>>>>
void myMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        corner[0].x = x;
        corner[0].y = screenHeight-y;
        selected = true;
    }
    glutPostRedisplay();
}

//<<<<<<<<<<<<<<<<<<<<<<<< myPassiveMotion >>>>>>>>>>>>>>>>>
void myPassiveMotion(int x, int y)
{
    corner[1].x = x;
    corner[1].y = screenHeight-y;
    glutPostRedisplay();
}

//<<<<<<<<<<<<<<<<<<<<<<<< main >>>>>>>>>>>>>>>>>>>>>>
int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    // 初始化窗口
    glutInitWindowSize(screenWidth, screenHeight);     // set window size
    glutInitWindowPosition(0, 0); // set window position on screen
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // set display mode

    // 创建窗口
    glutCreateWindow("Rubber Rect Demo"); // open the screen window

    // 设置投影矩阵
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity();
    gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);
    glMatrixMode(GL_MODELVIEW); 

    // 清渲染面
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);       // set black background color
    glViewport(0, 0, screenWidth, screenHeight);

    glutMouseFunc(myMouse);
    glutDisplayFunc(myDisplay);     // register redraw function
    glutPassiveMotionFunc(myPassiveMotion);
    glutMainLoop();              // go into a perpetual loop

    return 0;
}
