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

        if(t0>0)
        {
            return t0;
        }
        else
        {
            if(t1>0)
            {
                return t1;
            }
            else
            {
                return -1;
            }
        }

        return -1;
    }
};
