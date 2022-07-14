#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<bits/stdc++.h>

#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#define pi (2*acos(0.0))
using namespace std;

#define dbg(x) cout<<#x<<" -> "<<x<<endl;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct Point
{
    double x,y,z;
    Point(){}
    Point(double x,double y,double z)
    {
        this->x=x;
        this->y=y;
        this->z=z;
    }
    Point operator+(const Point& another)
    {
        return Point(x+another.x,y+another.y,z+another.z);
    }
    Point operator-(const Point& another)
    {
        return Point(x-another.x,y-another.y,z-another.z);
    }
    bool operator==(const Point& another)
    {
        return abs(x-another.x)<1e-9 && abs(y-another.y)<1e-9 && abs(z-another.z)<1e-9;
    }
    Point operator*(double v)
    {
        return Point(x*v,y*v,z*v);
    }
};
void print(Point a)
{
    cout<<setprecision(10)<<a.x<<' '<<a.y<<' '<<a.z<<endl;
}


// Vector ~ Point
#define Vector Point

const double R=20;
const double width=5;
Point center(0,0,R);
Vector normal(0,1,0);
double ANG=0; //For shading

double DOT(Vector a,Vector b)
{
    return a.x*b.x+a.y*b.y+a.z*b.z;
}
Vector CROSS(Vector a,Vector b)
{
    return Vector(a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x);
}

double LEN(Vector a)
{
    return sqrt(a.x*a.x+a.y*a.y+a.z*a.z);
}

Point ROTATE(Vector a,Vector b,double ang) /* rotate a around b by ang (degree)*/
{
    a=a*(1/LEN(a));
    b=b*(1/LEN(b));

    ang=ang*pi/180;
    Vector c=CROSS(b,a);
    c=c*(1/LEN(c));

    a=a*cos(ang);
    c=c*sin(ang);

    Vector res=a+c;
    res=res*(1/LEN(res));
    return res;
}


void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f( 100,0,0);
            glVertex3f(-100,0,0);

            glVertex3f(0,-100,0);
            glVertex3f(0, 100,0);

            glVertex3f(0,0, 100);
            glVertex3f(0,0,-100);
        }
        glEnd();
    }
}


void drawGrid()
{
    int i;
    if(drawgrid==1)
    {
        glColor3f(0.6, 0.6, 0.6);	//grey
        glBegin(GL_LINES);
        {
            for(i=-20; i<=20; i++)
            {

                glVertex3f(i*10, -210, 0);
                glVertex3f(i*10,  210, 0);

                glVertex3f(-210, i*10, 0);
                glVertex3f( 210, i*10, 0);
            }
        }
        glEnd();
    }
}


double fmod(double ang)
{
    while(ang<0) ang+=360;
    while(ang>=360) ang-=360;
    return ang;
}

void drawInnerRectangle(bool f)
{
    Vector radial_vector=ROTATE(normal,Vector(0,0,1),90);

    radial_vector=ROTATE(radial_vector,normal,ANG);
    if(f)
        radial_vector=ROTATE(radial_vector,normal,90);
    Point a=center+radial_vector*R;
    Point b=center+radial_vector*R;
    Point c=center-radial_vector*R;
    Point d=center-radial_vector*R;

    a=a+normal*width;
    b=b-normal*width;
    c=c+normal*width;
    d=d-normal*width;
    glBegin(GL_QUADS);
    {
        glColor3f(0.6,0.6,0.6);
        glVertex3f(a.x,a.y,a.z);
        glVertex3f(b.x,b.y,b.z);
        glVertex3f(d.x,d.y,d.z);
        glVertex3f(c.x,c.y,c.z);
    }
    glEnd();
}

void drawWheel(int slices,int stacks)
{
    struct Point points[100][100];

    /*Left part*/
    for(int i=0;i<=stacks;i++)
    {
        for(int j=0;j<=slices;j++)
        {
            Point temp_center=center+normal*(i*1.0/stacks)*width;
            Vector radial_vector=ROTATE(normal,Vector(0,0,1),90);

            radial_vector=ROTATE(radial_vector,normal,ANG+(j*1.0/slices)*360);

            points[i][j]=temp_center+radial_vector*R;
            //print(points[i][j]);
        }
    }
    for(int i=0;i<stacks;i++)
    {
        for(int j=0;j<slices;j++)
        {
            double shade;

            if(j<slices/2)
                shade=2*(double)j/(double)slices;
            else
                shade=2*(1.0-(double)j/(double)slices);

            glColor3f(shade,shade,shade);
            glBegin(GL_QUADS);
            {
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
            }
            glEnd();
        }
    }

    /*Right part*/
    for(int i=0;i<=stacks;i++)
    {
        for(int j=0;j<=slices;j++)
        {
            Point temp_center=center+(Vector(0,0,0)-normal)*(i*1.0/stacks)*width;
            Vector radial_vector=ROTATE(normal,Vector(0,0,1),90);

            radial_vector=ROTATE(radial_vector,normal,ANG+(j*1.0/slices)*360);

            points[i][j]=temp_center+radial_vector*R;
            //print(points[i][j]);
        }
    }
    for(int i=0;i<stacks;i++)
    {
        for(int j=0;j<slices;j++)
        {
            double shade;

            if(j<slices/2)
                shade=2*(double)j/(double)slices;
            else
                shade=2*(1.0-(double)j/(double)slices);

            glColor3f(shade,shade,shade);
            glBegin(GL_QUADS);
            {
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
            }
            glEnd();
        }
    }

    /*First rectangle*/
    drawInnerRectangle(0);
    /*Second rectangle*/
    drawInnerRectangle(1);
}


void keyboardListener(unsigned char key, int x,int y)
{
    Vector l;
    double prog;

    switch(key)
    {
    case 'a':
        normal=ROTATE(normal,Vector(0,0,1),3);
        break;
    case 'd':
        normal=ROTATE(normal,Vector(0,0,1),-3);
        break;

    case 's':
        ANG=fmod(ANG-5);
        l=ROTATE(normal,Vector(0,0,1),-90);
        prog=2*pi*R*5.0/360;
        center=center-l*prog;
        break;

    case 'w':
        ANG=fmod(ANG+5);
        l=ROTATE(normal,Vector(0,0,1),-90);
        prog=2*pi*R*5.0/360;
        center=center+l*prog;
        break;
    default:
        break;
    }
}


void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN:		//down arrow key
        cameraHeight -= 3.0;

        break;
    case GLUT_KEY_UP:		// up arrow key
        cameraHeight += 3.0;

        break;

    case GLUT_KEY_RIGHT:
        cameraAngle += 0.03;


        break;
    case GLUT_KEY_LEFT:
        cameraAngle -= 0.03;

        break;
    default:
        break;
    }
}


void mouseListener(int button, int state, int x, int y) 	//x, y is the x-y of the screen (2D)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN) 		// 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
            drawaxes=1-drawaxes;
        }
        break;

    case GLUT_RIGHT_BUTTON:
        //........
        break;

    case GLUT_MIDDLE_BUTTON:
        //........
        break;

    default:
        break;
    }
}

void display()
{

    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    //initialize the matrix
    glLoadIdentity();

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?

    gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);

    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);

    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    drawAxes();
    drawGrid();

    glPushMatrix();
    drawWheel(40,50);
    glPopMatrix();


    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate()
{
    angle+=0.05;
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init()
{
    //codes for initialization
    drawgrid=1;
    drawaxes=0;
    cameraHeight=150.0;
    cameraAngle=1.0;
    angle=0;

    //clear the screen
    glClearColor(0,0,0,0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(80,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("Task3");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}
