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
};
