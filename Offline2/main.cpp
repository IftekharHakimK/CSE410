#include<bits/stdc++.h>
#define M 1000000007
using namespace std;

#ifndef ONLINE_JUDGE
#include "C:\Users\iftek\Documents\dbg.h"
#define debug(...) debug_out(vec_splitter(#__VA_ARGS__), 0, __LINE__, __VA_ARGS__)
#else
#define debug(...)
#endif

#define pi acos(-1.0)

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
    Point operator/(double v)
    {
        return Point(x/v,y/v,z/v);
    }
};
//void print(Point a)
//{
//    cout<<setprecision(10)<<a.x<<' '<<a.y<<' '<<a.z<<endl;
//}
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

#define N 4

#define MATRIX vector<vector<double>>

MATRIX getI4()
{
    MATRIX res;
    res.resize(N);
    for(int i=0;i<N;i++)
        res[i].resize(N);

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            res[i][j]=(i==j);
        }
    }
    return res;
}

MATRIX multiply(MATRIX a,MATRIX b)
{

    int n1=a.size(),m1=a[0].size();
    int n2=b.size(),m2=a[0].size();
    assert(m1==n2);

    MATRIX res;
    res.resize(n1);
    for(int i=0;i<n1;i++)
        res[i].resize(m2);

    for(int i=0;i<n1;i++)
    {
        for(int j=0;j<m2;j++)
        {
            res[i][j]=0;
            for(int k=0;k<m1;k++)
            {
                res[i][j]+=a[i][k]*b[k][j];
            }
        }
    }
    return res;
}

Vector R(Vector X,Vector A,double theta) //theta in degree
{
    theta=theta*pi/180.0;
    Vector ans=X*cos(theta)+A*DOT(A,X)*(1-cos(theta))+CROSS(A,X)*sin(theta);
    return ans;
}

int cnt;
Point eye;
Vector look;
Vector up;
double fovY,aspectRatio,near,far;
ofstream out;
ifstream in;

void Stage1()
{

    in.open("scene.txt");
    out.open("stage1.txt");

    in>>eye.x>>eye.y>>eye.z;
    in>>look.x>>look.y>>look.z;
    in>>up.x>>up.y>>up.z;
    in>>fovY>>aspectRatio>>near>>far;

    stack<MATRIX>S;
    S.push(getI4());
    string command;
    while(in>>command)
    {
        if(command=="triangle")
        {
            for(int i=0;i<3;i++)
            {
                MATRIX v;
                v.resize(4);
                for(int i=0;i<4;i++)
                    v[i].resize(1);
                in>>v[0][0]>>v[1][0]>>v[2][0];
                v[3][0]=1;

                v=multiply(S.top(),v);
                for(int i=0;i<3;i++)
                    out<<v[i][0]<<' ';
                out<<'\n';
            }
            out<<'\n';
            cnt+=3;
        }
        else if(command=="translate")
        {
            MATRIX v=getI4();
            in>>v[0][3]>>v[1][3]>>v[2][3];
            auto u=S.top();
            S.pop();
            S.push(multiply(u,v));
        }
        else if(command=="scale")
        {
            MATRIX v=getI4();
            in>>v[0][0]>>v[1][1]>>v[2][2];
            auto u=S.top();
            S.pop();
            S.push(multiply(u,v));
        }
        else if(command=="rotate")
        {
            double ang;
            in>>ang;
            Vector A;
            in>>A.x>>A.y>>A.z;
            A=A/LEN(A);

            MATRIX v=getI4();

            for(int i=0;i<3;i++)
            {
                Vector c=R(Vector(i==0,i==1,i==2),A,ang);
                v[0][i]=c.x;
                v[1][i]=c.y;
                v[2][i]=c.z;
            }
            auto u=S.top();
            S.pop();
            S.push(multiply(u,v));

        }
        else if(command=="push")
        {
            S.push(S.top());
        }
        else if(command=="pop")
        {
            S.pop();
        }
        else if(command=="end")
        {
            break;
        }
    }
    in.close();
    out.close();
}

void Stage2()
{
    in.open("stage1.txt");
    out.open("stage2.txt");

    Vector l=look-eye;
    l=l/LEN(l);
    Vector r=CROSS(l,up);
    r=r/LEN(r);
    Vector u=CROSS(r,l);

    MATRIX T=getI4();
    T[0][3]=-eye.x;
    T[1][3]=-eye.y;
    T[2][3]=-eye.z;

    MATRIX R=getI4();
    R[0]={r.x,r.y,r.z,0};
    R[1]={u.x,u.y,u.z,0};
    R[2]={-l.x,-l.y,-l.z,0};

    MATRIX V=multiply(R,T);

    MATRIX pt;
    pt.resize(4);
    for(int i=0;i<4;i++)
        pt[i].resize(1);
    pt[3][0]=1;
    for(int cs=1;cs<=cnt;cs++)
    {
        in>>pt[0][0]>>pt[1][0]>>pt[2][0];
        pt[3][0]=1;

        pt=multiply(V,pt);


        out<<pt[0][0]<<' '<<pt[1][0]<<' '<<pt[2][0]<<'\n';
        if(cs%3==0)
            out<<'\n';
    }

    in.close();
    out.close();
}

void Stage3()
{
    in.open("stage2.txt");
    out.open("stage3.txt");

    fovY=fovY*pi/180.0;

    double fovX=fovY*aspectRatio;
    double t=near*tan(fovY/2);
    double r=near*tan(fovX/2);

    MATRIX P;
    P.resize(4);
    for(int i=0;i<4;i++)
        P[i].resize(4);

    P[0][0]=near/r;
    P[1][1]=near/t;
    P[2][2]=-(far+near)/(far-near);
    P[2][3]=-(far*near*2)/(far-near);
    P[3][2]=-1;

    MATRIX pt;
    pt.resize(4);
    for(int i=0;i<4;i++)
        pt[i].resize(1);
    pt[3][0]=1;
    for(int cs=1;cs<=cnt;cs++)
    {
        in>>pt[0][0]>>pt[1][0]>>pt[2][0];
        pt[3][0]=1;

        pt=multiply(P,pt);
        pt[0][0]/=pt[3][0];
        pt[1][0]/=pt[3][0];
        pt[2][0]/=pt[3][0];
        pt[3][0]/=pt[3][0];

        out<<pt[0][0]<<' '<<pt[1][0]<<' '<<pt[2][0]<<'\n';
        if(cs%3==0)
            out<<'\n';
    }

    in.close();
    out.close();
}

main()
{
//    ios_base::sync_with_stdio(0);
//    cin.tie(0);

    out<<fixed<<setprecision(7);

    Stage1();
    Stage2();
    Stage3();

}
