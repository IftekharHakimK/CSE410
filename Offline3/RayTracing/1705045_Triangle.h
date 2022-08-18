extern vector<PointLight>pointLights;
extern vector<SpotLight>spotLights;
extern vector<Object*>objects;
extern int recursionLevel;

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
        double v1[9]= {vertex[0].x-ray.Ro.x,vertex[0].x-vertex[2].x,ray.Rd.x,
                       vertex[0].y-ray.Ro.y,vertex[0].y-vertex[2].y,ray.Rd.y,
                       vertex[0].z-ray.Ro.z,vertex[0].z-vertex[2].z,ray.Rd.z
                      };
        Matrix33 m_beta=Matrix33(v1);

        double v2[9]= {vertex[0].x-vertex[1].x,vertex[0].x-ray.Ro.x,ray.Rd.x,
                       vertex[0].y-vertex[1].y,vertex[0].y-ray.Ro.y,ray.Rd.y,
                       vertex[0].z-vertex[1].z,vertex[0].z-ray.Ro.z,ray.Rd.z
                      };
        Matrix33 m_gamma=Matrix33(v2);

        double v3[9]= {vertex[0].x-vertex[1].x,vertex[0].x-vertex[2].x,vertex[0].x-ray.Ro.x,
                       vertex[0].y-vertex[1].y,vertex[0].y-vertex[2].y,vertex[0].y-ray.Ro.y,
                       vertex[0].z-vertex[1].z,vertex[0].z-vertex[2].z,vertex[0].z-ray.Ro.z
                      };
        Matrix33 m_t=Matrix33(v3);

        double v4[9]= {vertex[0].x-vertex[1].x,vertex[0].x-vertex[2].x,ray.Rd.x,
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
        if(level==0||t<0)
            return t;

        Point intersectionPoint=ray.Ro+ray.Rd*t;

        double intersectionPointColor[3]= {0,0,0};

        intersectionPointColor[0]=this->color[0];
        intersectionPointColor[1]=this->color[1];
        intersectionPointColor[2]=this->color[2];

        color[0]=intersectionPointColor[0]*ambient;
        color[1]=intersectionPointColor[1]*ambient;
        color[2]=intersectionPointColor[2]*ambient;

        Vector normal=CROSS(vertex[1]-vertex[0],vertex[2]-vertex[0]);
        normal=normal/LEN(normal);


        if(DOT(normal,ray.Rd*-1)<0)
        {
            normal=normal*-1;
        }

        for(PointLight pl:pointLights)
        {
            Ray L(pl.pos,intersectionPoint-pl.pos);

            double t_block=INF;
            for(auto u:objects)
            {
                double dummy_color[3]= {0,0,0};
                double t_here=u->intersect(L,dummy_color,0);
                if(t_here>0&&t_here<t_block)
                {
                    t_block=t_here;
                }
            }

            if(t_block>0)
            {
                Point blocked_at=L.Ro+L.Rd*t_block;
                if(!isSamePoint(blocked_at,intersectionPoint))
                {
                    continue;
                }
            }


            Vector uL=L.Rd*-1;

            double lambert=DOT(uL,normal);
            Vector R=normal*DOT(uL,normal)*2-uL;
            R=R/LEN(R);

            double phong=DOT(ray.Rd*-1,R);

            lambert=max(0.0,lambert);
            phong=max(0.0,phong);

            color[0]+=pl.color[0]*diffuse*lambert*intersectionPointColor[0];
            color[1]+=pl.color[1]*diffuse*lambert*intersectionPointColor[1];
            color[2]+=pl.color[2]*diffuse*lambert*intersectionPointColor[2];

            color[0]+=pl.color[0]*specular*pow(phong,shine)*intersectionPointColor[0];
            color[1]+=pl.color[1]*specular*pow(phong,shine)*intersectionPointColor[1];
            color[2]+=pl.color[2]*specular*pow(phong,shine)*intersectionPointColor[2];
//            dbg(lambert);
//            dbg(phong);
        }

        for(SpotLight sl:spotLights)
        {
            Ray L(sl.pos,intersectionPoint-sl.pos);

            //check cutoff
            double angle=getAngleInDegree(sl.light_direction,L.Rd);
            if(angle>sl.cutoff_angle)
            {
                continue;
            }


            double t_block=INF;
            for(auto u:objects)
            {
                double dummy_color[3]= {0,0,0};
                double t_here=u->intersect(L,dummy_color,0);
                if(t_here>0&&t_here<t_block)
                {
                    t_block=t_here;
                }
            }

            if(t_block>0)
            {
                Point blocked_at=L.Ro+L.Rd*t_block;
                if(!isSamePoint(blocked_at,intersectionPoint))
                {
                    continue;
                }
            }

            Vector uL=L.Rd*-1;

            double lambert=DOT(uL,normal);
            Vector R=normal*DOT(uL,normal)*2-uL;
            R=R/LEN(R);

            double phong=DOT(ray.Rd*-1,R);

            lambert=max(0.0,lambert);
            phong=max(0.0,phong);

            color[0]+=sl.color[0]*diffuse*lambert*intersectionPointColor[0];
            color[1]+=sl.color[1]*diffuse*lambert*intersectionPointColor[1];
            color[2]+=sl.color[2]*diffuse*lambert*intersectionPointColor[2];

            color[0]+=sl.color[0]*specular*pow(phong,shine)*intersectionPointColor[0];
            color[1]+=sl.color[1]*specular*pow(phong,shine)*intersectionPointColor[1];
            color[2]+=sl.color[2]*specular*pow(phong,shine)*intersectionPointColor[2];
//            dbg(lambert);
//            dbg(phong);
//            cout<<"here"<<endl;
        }
        if(level==recursionLevel)
            return t;

        Vector uL=ray.Rd*-1;
        Vector R=normal*DOT(uL,normal)*2-uL;
        Ray reflectRay(intersectionPoint+R*EPS*3,R);
        Object * which=NULL;
        double tMin=INF;

        for(Object * obj:objects)
        {
            double dummy[3]= {0,0,0};
            double t_r=obj->intersect(reflectRay,dummy,0);
//                dbg(t);

            if(t_r>0&&t_r<tMin)
            {
                tMin=t_r;
                which=obj;
            }
        }

        if(which!=NULL)
        {
            double reflectedColor[3]= {0,0,0};
            which->intersect(reflectRay,reflectedColor,level+1);

            color[0]+=reflectedColor[0]*reflection;
            color[1]+=reflectedColor[1]*reflection;
            color[2]+=reflectedColor[2]*reflection;

        }

        color[0]=min(1.0,max(0.0,color[0]));
        color[1]=min(1.0,max(0.0,color[1]));
        color[2]=min(1.0,max(0.0,color[2]));

        return t;
    }
};

