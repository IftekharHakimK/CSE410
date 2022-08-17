class Object
{
public:

    double color[3];

    //co-efficient
    double ambient;
    double diffuse;
    double specular;
    double reflection;

    int shine; // exponent term of specular component
    Object(){}
    virtual void draw()=0;
//    {
//        cout<<": ("<<endl;
//    }
    void setColor(double r,double g,double b)
    {
        color[0]=r;
        color[1]=g;
        color[2]=b;
    }
    void setShine(int _shine)
    {
        shine=_shine;
    }
    void setAmbient(double _ambient)
    {
        ambient=_ambient;
    }
    void setDiffuse(double _diffuse)
    {
        diffuse=_diffuse;
    }
    void setSpecular(double _specular)
    {
        specular=_specular;
    }
    void setReflection(double _reflection)
    {
        reflection=_reflection;
    }
};
