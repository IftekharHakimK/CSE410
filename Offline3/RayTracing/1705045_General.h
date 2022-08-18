extern vector<PointLight>pointLights;
extern vector<SpotLight>spotLights;
extern vector<Object*>objects;
extern int recursionLevel;

class General: public Object
{
public:

    Point center;
    double A,B,C,D,E,F,G,H,I,J;
    Point ref_point;
    double height, width, length;

    General(double _A,double _B,double _C,double _D,double _E, double _F, double _G, double _H, double _I, double _J, Point _ref, double _h, double _w, double _l)
    {
        A=_A;
        B=_B;
        C=_C;
        D=_D;
        E=_E;
        F=_F;
        G=_G;
        H=_H;
        I=_I;
        J=_J;
        ref_point=_ref;
        height=_h;
        width=_w;
        length=_l;
    }
    void draw()
    {

    }
    double intersect(Ray ray, double * color, int level)
    {
        Vector Rd=ray.Rd;
        Point Ro=ray.Ro;

        double a=A*Rd.x*Rd.x+B*Rd.y*Rd.y+C*Rd.z*Rd.z+D*Rd.x*Rd.y+E*Rd.x*Rd.z+F*Rd.y*Rd.z;
        double b=2.0*A*Ro.x*Rd.x+2*B*Ro.y*Rd.y+2*C*Ro.z*Rd.z+D*(Ro.x*Rd.y+Ro.y*Rd.x)+E*(Ro.x*Rd.z+Ro.z*Rd.x)+F*(Ro.y*Rd.z+Rd.y*Ro.z)+G*Rd.x+H*Rd.y+I*Rd.z;
        double c=A*Ro.x*Ro.x+B*Ro.y*Ro.y+C*Ro.z*Ro.z+D*Ro.x*Ro.y+E*Ro.x*Ro.z+F*Ro.y*Ro.z+G*Ro.x+H*Ro.y+I*Ro.z+J;

        double t0=-1,t1=-1;

        if(isSame(a,0))
        {
            if(b==0)
                t0=t1=-1;
            else
                t0=t1=-c/b;
        }
        else
        {
            double d=b*b-4*a*c;
            if(d<0)
            {
                t0=t1=-1;
            }
            else
            {
                t0=(-b-sqrt(b*b-4*a*c))/(a*2);
                t1=(-b+sqrt(b*b-4*a*c))/(a*2);
            }
        }

        Point p0=Ro+Rd*t0;
        Point p1=Ro+Rd*t1;

        if(!isSame(length,0)&&(p0.x<ref_point.x||p0.x>ref_point.x+length))
            t0=-1;
        if(!isSame(width,0)&&(p0.y<ref_point.y||p0.y>ref_point.y+width))
            t0=-1;
        if(!isSame(height,0)&&(p0.z<ref_point.z||p0.z>ref_point.y+height))
            t0=-1;

        if(!isSame(length,0)&&(p1.x<ref_point.x||p1.x>ref_point.x+length))
            t1=-1;
        if(!isSame(width,0)&&(p1.y<ref_point.y||p1.y>ref_point.y+width))
            t1=-1;
        if(!isSame(height,0)&&(p1.z<ref_point.z||p1.z>ref_point.y+height))
            t1=-1;

        color[0]=this->color[0];
        color[1]=this->color[1];
        color[2]=this->color[2];

        double t=-1;

        if(t0>0)
        {
            t=t0;
        }
        else
        {
            if(t1>0)
            {
                t=t1;
            }
        }
        if(level==0||t<0)
        {
            return t;
        }

        Point intersectionPoint=ray.Ro+ray.Rd*t;

        double intersectionPointColor[3]= {0,0,0};

        intersectionPointColor[0]=this->color[0];
        intersectionPointColor[1]=this->color[1];
        intersectionPointColor[2]=this->color[2];

        color[0]=intersectionPointColor[0]*ambient;
        color[1]=intersectionPointColor[1]*ambient;
        color[2]=intersectionPointColor[2]*ambient;

        Vector normal;

        normal.x=2*A*intersectionPoint.x+D*intersectionPoint.y+E*intersectionPoint.z+G;
        normal.y=2*B*intersectionPoint.y+D*intersectionPoint.x+F*intersectionPoint.z+H;
        normal.z=2*C*intersectionPoint.z+E*intersectionPoint.x+F*intersectionPoint.y+I;


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
//            cout<<"blocked"<<endl;

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
