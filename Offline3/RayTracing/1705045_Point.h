class Point
{
public:

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
    Point operator/(double v)
    {
        return Point(x/v,y/v,z/v);
    }
};
#define Vector Point
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
void printPoint(Point p)
{
    cout<<p.x<<' '<<p.y<<' '<<p.z<<endl;
}
bool isSame(double a,double b)
{
    return fabs(a-b)<EPS;
}
