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
const double L= 30;
double sq=22;


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

Point pos(100,0,0);

// Vector ~ Point
#define Vector Point
Vector u(0,0,1);
Vector r(0,1,0);
Vector l(-1,0,0);

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
            for(i=-8; i<=8; i++)
            {

                if(i==0)
                    continue;	//SKIP the MAIN axes

                //lines parallel to Y-axis
                glVertex3f(i*10, -90, 0);
                glVertex3f(i*10,  90, 0);

                //lines parallel to X-axis
                glVertex3f(-90, i*10, 0);
                glVertex3f( 90, i*10, 0);
            }
        }
        glEnd();
    }
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);
    {
        glVertex3f( a, a,0);
        glVertex3f( a,-a,0);
        glVertex3f(-a,-a,0);
        glVertex3f(-a, a,0);
    }
    glEnd();
}

void drawOneFourthCylinder(double radius,double height,int slices,int stacks)
{
    glTranslatef(sq,sq,-sq);

    Point point[100][100];
    for(int i=0;i<=stacks;i++)
    {
        for(int j=0;j<=slices;j++)
        {
            double ang=i*1.0/slices*(pi/2);
            point[i][j]=Point(radius*cos(ang),radius*sin(ang),height*j/stacks);
        }
    }
    glColor3f(0,1,0);
    for(int i=0;i<stacks;i++)
    {
        for(int j=0;j<slices;j++)
        {
            //cout<<"here"<<endl;
            glBegin(GL_QUADS);
            {
                glVertex3f(point[i][j].x,point[i][j].y,point[i][j].z);
                glVertex3f(point[i][j+1].x,point[i][j+1].y,point[i][j+1].z);
                glVertex3f(point[i+1][j+1].x,point[i+1][j+1].y,point[i+1][j+1].z);
                glVertex3f(point[i+1][j].x,point[i+1][j].y,point[i+1][j].z);
            }
            glEnd();
        }
    }

}

void drawOneEighthSphere(double radius,int slices,int stacks)
{
    glTranslatef(sq,sq,sq);

    struct Point points[100][100];
    double h,r;
    //generate points
    for(int i=0; i<=stacks; i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(int j=0; j<=slices; j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*pi/2);
            points[i][j].y=r*sin(((double)j/(double)slices)*pi/2);
            points[i][j].z=h;
        }
    }
    glColor3f(1,0,0);
    for(int i=0; i<stacks; i++)
    {
        for(int j=0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
            }
            glEnd();
        }
    }
}

void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {

    case '1':
        l=ROTATE(l,u,5);
        r=ROTATE(r,u,5);
        break;
    case '2':
        l=ROTATE(l,u,-5);
        r=ROTATE(r,u,-5);
        break;

    case '3':
        l=ROTATE(l,r,5);
        u=ROTATE(u,r,5);
        break;
    case '4':
        l=ROTATE(l,r,-5);
        u=ROTATE(u,r,-5);
        break;
    case '5':
        u=ROTATE(u,l,5);
        r=ROTATE(r,l,5);

        break;
    case '6':
        u=ROTATE(u,l,-5);
        r=ROTATE(r,l,-5);

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

        pos=pos-l*2;

        break;
    case GLUT_KEY_UP:		// up arrow key
        cameraHeight += 3.0;

        pos=pos+l*2;

        break;

    case GLUT_KEY_RIGHT:
        cameraAngle += 0.03;

        pos=pos+r*2;

        break;
    case GLUT_KEY_LEFT:
        cameraAngle -= 0.03;

        pos=pos-r*2;
        break;

    case GLUT_KEY_PAGE_UP:

        pos=pos+u*2;

        break;
    case GLUT_KEY_PAGE_DOWN:
        pos=pos-u*2;
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        sq=max(sq-1,0.0);
        break;
    case GLUT_KEY_END:
        sq=min(sq+1,L);
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

void draw4cylinders()
{
    glPushMatrix();
    drawOneFourthCylinder(L-sq,sq*2,50,50);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90,0,0,1);
    drawOneFourthCylinder(L-sq,sq*2,50,50);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180,0,0,1);
    drawOneFourthCylinder(L-sq,sq*2,50,50);
    glPopMatrix();

    glPushMatrix();
    glRotatef(270,0,0,1);
    drawOneFourthCylinder(L-sq,sq*2,50,50);
    glPopMatrix();
}

void draw4spheres()
{
    glPushMatrix();
    drawOneEighthSphere(L-sq,50,50);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90,0,0,1);
    drawOneEighthSphere(L-sq,50,50);
    glPopMatrix();

    glPushMatrix();
    glRotatef(180,0,0,1);
    drawOneEighthSphere(L-sq,50,50);
    glPopMatrix();

    glPushMatrix();
    glRotatef(270,0,0,1);
    drawOneEighthSphere(L-sq,50,50);
    glPopMatrix();

}

void draw2squares(double a)
{
    glPushMatrix();
    glTranslatef(0,0,-L);
    drawSquare(a);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,0,L);
    drawSquare(a);
    glPopMatrix();

}

void display()
{

    assert(DOT(u,r)<1e-9);
    assert(DOT(r,l)<1e-9);
    assert(DOT(l,u)<1e-9);

    assert(CROSS(r,l)==u);
    assert(CROSS(u,r)==l);
    assert(CROSS(l,u)==r);

    assert(abs(LEN(u)-1)<1e-9);
    assert(abs(LEN(r)-1)<1e-9);
    assert(abs(LEN(l)-1)<1e-9);

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

    Point dir=pos+l;

    gluLookAt(pos.x,pos.y,pos.z,	dir.x,dir.y,dir.z,	u.x,u.y,u.z);

    glMatrixMode(GL_MODELVIEW);

    drawAxes();
    drawGrid();

    glColor3f(255,255,255);


    /* 6 sqaures. Draw 2 and rotate accordingly*/
    glPushMatrix();
    draw2squares(sq);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90,1,0,0);
    draw2squares(sq);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90,0,1,0);
    draw2squares(sq);
    glPopMatrix();

    /* end of squares */

    /* 12 one-fourth cylinders. Draw 4 and rotate accordingly */

    glPushMatrix();
    glRotatef(90,1,0,0);
    draw4cylinders();
    glPopMatrix();

    glPushMatrix();
    glRotatef(90,0,1,0);
    draw4cylinders();
    glPopMatrix();

    glPushMatrix();
    draw4cylinders();
    glPopMatrix();

    /* end of cylinders */


    /* 8 one-eighth spheres. Draw 4 and rotate accordingly */

    glPushMatrix();
    draw4spheres();
    glPopMatrix();

    glPushMatrix();
    glRotatef(180,0,1,0);
    draw4spheres();
    glPopMatrix();

    /* end of spheres */

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
    drawgrid=0;
    drawaxes=1;
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

    glutCreateWindow("My OpenGL Program");

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
