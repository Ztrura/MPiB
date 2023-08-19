
#include <iostream>
#include <Windows.h>
#include "ObjLoader.h"
#include <GL/glui.h>
using namespace std;

//double x[] = { -1.0, 1.0 }, y[] = { -1.0, 1.0 }, z[] = { -1.0, 1.0 };

string filepath;
ObjLoader objModel;

float radius;

const int width = 1000;
const int height = 700;

static float c = 3.1415926 / 180.0f;

float xrot = 20.0f;
float yrot = -20.0f;

float xdiff = 0.0f;
float ydiff = 0.0f;

//float times = 1.0f;
float times = 0.7f;
float tranX1 = 0.0f;
float tranY1 = 0.0f;

//初始位置
float tranX = 0.0f;
float tranY = 0.0f;
float tranZ = -15.0f;
float mouseX, mouseY;
bool leftMouse = false;
bool middleMouse = false;

int mode = 0;
int circle = 1;
int axes = 1;
int grid = 1;

float screenX = 0, screenY = 0;

void setLightRes() {
    GLfloat AmbientColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };//红、绿、蓝、透明度
    glLightfv(GL_LIGHT0, GL_AMBIENT, AmbientColor);//环境光

    GLfloat lightPosition1[] = { 0.0f, 0.0f, -25.0f, 1.0f };
    GLfloat lightPosition2[] = { -25.0f, 0.0f, 0.0f, 1.0f };
    GLfloat lightPosition3[] = { 25.0f, 0.0f, 0.0f, 1.0f };
    GLfloat lightColor1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightColor2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat lightColor3[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);

    glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor2);

    glLightfv(GL_LIGHT3, GL_POSITION, lightPosition3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColor3);

    glEnable(GL_LIGHTING); //启用光源
    glEnable(GL_LIGHT0);   //启用环境光
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);
}

//初始化
void init() {
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(width, height);
    glutCreateWindow("GYLab");
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    setLightRes();
    glEnable(GL_DEPTH_TEST);
}

void draw_axes(float scale)
{
    glDisable(GL_LIGHTING);

    glPushMatrix();
    glScalef(scale, scale, scale);

    glLineWidth(2.5f);
    glBegin(GL_LINES);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(4.8f, 0.15f, 0.0);  glVertex3f(5.0, 0.35f, 0.0); /* Letter X*/
    glVertex3f(4.8f, 0.35f, 0.0);  glVertex3f(5.0, 0.15f, 0.0);

    glVertex3f(0.0, 0.0, 0.0);  glVertex3f(5.0, 0.0, 0.0); /* X axis */

    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);  glVertex3f(0.0, 5.0, 0.0); /* Y axis */

    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);  glVertex3f(0.0, 0.0, 5.0); /* Z axis  */

    glTranslatef(tranX1, tranY1, 0.0f);
    glEnd();
    glLineWidth(1.0f);

    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void gridding(float scale) {
    glDisable(GL_LIGHTING);

    glPushMatrix();
    glScalef(scale, scale, scale);

    glBegin(GL_LINES);

    glColor3f(0.5, 0.5, 0.5);
    for (int i = -1000; i <= 1000; i+=2) {
        glVertex3f(-1000.0, 0.0, i);  glVertex3f(1000.0, 0.0, i);
    }
    
    glColor3f(0.5, 0.5, 0.5);
    for (int i = -1000; i <= 1000; i+=2) {
        glVertex3f(i, 0.0, -1000.0);  glVertex3f(i, 0.0, 1000.0); 
    }

    glTranslatef(tranX1, tranY1, 0.0f);
    glEnd();

    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void draw_circle(float R) {
    glDisable(GL_LIGHTING);
    int n = 100;
    float Pi = 3.1415926f;
    glBegin(GL_POLYGON);
    glColor3f(0.9, 0.9, 0.9);
    for (int i = 0; i < n; i++) {
        glVertex3f(R * cos(2 * Pi / n * i), 0, R * sin(2 * Pi / n * i));
    }
    glEnd();
    glFlush();
    glEnable(GL_LIGHTING);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
    case 'W':
    {
        tranY += 0.02f;
        glutPostRedisplay();
        break;
    }
    case 's':
    case 'S':
    {
        tranY += -0.02f;
        glutPostRedisplay();
        break;
    }
    case 'a':
    case 'A':
    {
        tranX += -0.02f;
        glutPostRedisplay();
        break;
    }
    case 'd':
    case 'D':
    {
        tranX += 0.02f;
        glutPostRedisplay();
        break;
    }
    case 'e':
    case 'E':
    {
        tranZ += 0.02f;
        glutPostRedisplay();
        break;
    }
    case 'q':
    case 'Q':
    {
        tranZ += -0.02f;
        glutPostRedisplay();
        break;
    }
    case 27:
        exit(0);
        break;
    }
}

void newdisplay()
{
    glClearColor(0.8, 0.8, 0.8, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, 1.0f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(tranX, tranY, tranZ);
    glScalef(times, times, times);
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);
    if (circle == 1) {
        radius = 0.11 * objModel.maxR;
        draw_circle(radius);
    }
    if (mode == 1) objModel.Draw();
    if (mode == 2) objModel.checkOutlier();
    if (mode == 3) objModel.removeOutlier();
    if (mode == 4) objModel.pointReshape();
    if (mode == 5) objModel.Map();
    if (mode == 6) objModel.Link();
    if (mode == 7) objModel.lineReshape();
    if (mode == 8) objModel.lineReshape_WP();
    //if (axes == 1) draw_axes(.52f);
    if (axes == 1) draw_axes(.73f);
    if (grid == 1) gridding(.73f);
    glPopMatrix();
    glutSwapBuffers();//双缓冲    
}

void mouseMove(int button, int state, int x, int y) {
    mouseX = x;
    mouseY = y;
    screenX = x;
    screenY = y;
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        leftMouse = true;
        xdiff = x - yrot;
        ydiff = -y + xrot;
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        leftMouse = false;
    }
    else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN) {
        middleMouse = true;
        std::cout << "x = " << x << " y = " << y << std::endl;
    }
    else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP) {
        middleMouse = false;
    }
    else if (button == GLUT_WHEEL_UP && state == GLUT_UP)
    {
        times += 0.08f;
        glutPostRedisplay();
    }

    else if (button == GLUT_WHEEL_DOWN && state == GLUT_UP)
    {
        times -= 0.08f;
    }
    else {
        leftMouse = false;
    }
}

void changeViewPoint(int x, int y) {
    //tranX = tranY = 0;
    if (leftMouse) {
        yrot = x - xdiff;
        xrot = y + ydiff;
    }
    if (middleMouse)
    {
        tranX += (x - mouseX) * 0.02f;
        tranY += -(y - mouseY) * 0.02f;
        mouseY = y;
        mouseX = x;
    }
    glutPostRedisplay();
}

void myIdle()
{
    glutPostRedisplay();
}

//#define OHP 1 //正投影
//#define PER 2 //透视投影
#define INI 1   //init
#define CHE 2   //check
#define REM 3   //remove
#define REP 4   //reshape_line
#define MAP 5   //map
#define LIN 6   //link
#define REL 7   //reshape_line
#define RWP 8   //reshape_line_without_point
#define CIR_ON 9
#define CIR_OFF 10
#define AXES_ON 11
#define AXES_OFF 12
#define GRID_ON 13
#define GRID_OFF 14

void processMenuEvents(int option) {

    switch (option) {
    case INI:
        mode = 1;
        break;
    case CHE:
        mode = 2;
        break;
    case REM:
        mode = 3;
        break;
    case REP:
        mode = 4;
        break;
    case MAP:
        mode = 5;
        break;
    case LIN:
        mode = 6;
        break;
    case REL:
        mode = 7;
        break;
    case RWP:
        mode = 8;
        break;
    case CIR_ON:
        circle = 1;
        break;
    case CIR_OFF:
        circle = 0;
        break;
    case AXES_ON:
        axes = 1;
        break;
    case AXES_OFF:
        axes = 0;
        break;
    case GRID_ON:
        grid = 1;
        break;
    case GRID_OFF:
        grid = 0;
        break;
    }
    glutPostRedisplay();
}

void createGLUTMenus() {

    int menu;

    //创建菜单并告诉GLUT，processMenuEvents处理菜单事件。
    menu = glutCreateMenu(processMenuEvents);

    //增加条目
    glutAddMenuEntry("init", INI);
    glutAddMenuEntry("check outlier", CHE);
    glutAddMenuEntry("remove outlier", REM);
    glutAddMenuEntry("reshape(point)", REP);
    glutAddMenuEntry("map", MAP);
    glutAddMenuEntry("link", LIN);
    glutAddMenuEntry("reshape(linked)", REL);
    glutAddMenuEntry("reshape(linked & without point)", RWP);
    glutAddMenuEntry("circle on", CIR_ON);
    glutAddMenuEntry("circle off", CIR_OFF);
    glutAddMenuEntry("axes on", AXES_ON);
    glutAddMenuEntry("axes off", AXES_OFF);
    glutAddMenuEntry("grid on", GRID_ON);
    glutAddMenuEntry("grid off", GRID_OFF);

    //把菜单和鼠标右键关联起来。
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char* argv[])
{
    //filepath = "D:\\R\\GYLab\\curve33.obj";
    filepath = "data/human.obj";
    objModel = ObjLoader(filepath);
    //cout << "max_radius: " << objModel.maxR << endl;
    glutInit(&argc, argv);
    init();
    glutDisplayFunc(newdisplay);
    createGLUTMenus();
    glutMouseFunc(mouseMove);
    glutMotionFunc(changeViewPoint);
    glutIdleFunc(myIdle);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
