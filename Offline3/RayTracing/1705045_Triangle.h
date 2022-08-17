class Triangle: public Object
{
public:

    Point vertex[3];

    Triangle(Point A,Point B,Point C)
    {
        vertex[0]=A;
        vertex[1]=B;
        vertex[2]=C;
    }
    void draw()
    {
        glColor3f(color[0],color[1],color[2]);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(vertex[0].x,vertex[0].y,vertex[0].z);
            glVertex3f(vertex[1].x,vertex[1].y,vertex[1].z);
            glVertex3f(vertex[2].x,vertex[2].y,vertex[2].z);
        }
        glEnd();
    }
};

