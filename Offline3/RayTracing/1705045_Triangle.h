
#include "1705045_Matrix.h"

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
        glPushMatrix();
        glColor3f(color[0],color[1],color[2]);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(vertex[0].x,vertex[0].y,vertex[0].z);
            glVertex3f(vertex[1].x,vertex[1].y,vertex[1].z);
            glVertex3f(vertex[2].x,vertex[2].y,vertex[2].z);
        }
        glEnd();
        glPopMatrix();
    }
    double intersect(Ray ray, double * color, int level)
    {
        double v1[9]={vertex[0].x-ray.Ro.x,vertex[0].x-vertex[2].x,ray.Rd.x,
                     vertex[0].y-ray.Ro.y,vertex[0].y-vertex[2].y,ray.Rd.y,
                     vertex[0].z-ray.Ro.z,vertex[0].z-vertex[2].z,ray.Rd.z
                     };
        Matrix33 m_beta=Matrix33(v1);

        double v2[9]={vertex[0].x-vertex[1].x,vertex[0].x-ray.Ro.x,ray.Rd.x,
                      vertex[0].y-vertex[1].y,vertex[0].y-ray.Ro.y,ray.Rd.y,
                      vertex[0].z-vertex[1].z,vertex[0].z-ray.Ro.z,ray.Rd.z
                     };
        Matrix33 m_gamma=Matrix33(v2);

        double v3[9]={vertex[0].x-vertex[1].x,vertex[0].x-vertex[2].x,vertex[0].x-ray.Ro.x,
                      vertex[0].y-vertex[1].y,vertex[0].y-vertex[2].y,vertex[0].y-ray.Ro.y,
                      vertex[0].z-vertex[1].z,vertex[0].z-vertex[2].z,vertex[0].z-ray.Ro.z
                     };
        Matrix33 m_t=Matrix33(v3);

        double v4[9]={vertex[0].x-vertex[1].x,vertex[0].x-vertex[2].x,ray.Rd.x,
                     vertex[0].y-vertex[1].y,vertex[0].y-vertex[2].y,ray.Rd.y,
                     vertex[0].z-vertex[1].z,vertex[0].z-vertex[2].z,ray.Rd.z
                     };
        Matrix33 m_A=Matrix33(v4);

        double A=m_A.determinant();

        if(isSame(A,0))
            return -1;

        double beta=m_beta.determinant()/A;
        double gamma=m_gamma.determinant()/A;
        double t=m_t.determinant()/A;

        if(!(beta+gamma<1&&beta>0&&gamma>0&&t>0))
            t=-1;
        assert(level==0);

        color[0]=this->color[0];
        color[1]=this->color[1];
        color[2]=this->color[2];

        return t;
    }
};

