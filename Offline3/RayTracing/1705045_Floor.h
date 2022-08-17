class Floor: public Object
{
public:
    double floorWidth;
    double tileWidth;

    Floor(double _floorWidth,double _tileWidth)
    {
        floorWidth=_floorWidth;
        tileWidth=_tileWidth;
    }
    void draw()
    {
//        return;
        glPushMatrix();
        Point bottomLeft(-floorWidth/2,-floorWidth/2,0);

        int cnt=ceil(floorWidth/tileWidth);
//        dbg(cnt);

        for(int i=0; i<cnt; i++)
        {
            for(int j=0; j<cnt; j++)
            {
//                cout<<i<<' '<<j<<endl;

                Point A=bottomLeft+Vector(1,0,0)*i*tileWidth+Vector(0,1,0)*j*tileWidth;
                Point B=bottomLeft+Vector(1,0,0)*(i+1)*tileWidth+Vector(0,1,0)*j*tileWidth;
                Point C=bottomLeft+Vector(1,0,0)*(i+1)*tileWidth+Vector(0,1,0)*(j+1)*tileWidth;
                Point D=bottomLeft+Vector(1,0,0)*i*tileWidth+Vector(0,1,0)*(j+1)*tileWidth;

                if((i+j)%2==0)
                {
                    glColor3f(1,1,1);
                }
                else
                {
                    glColor3f(0,0,0);
                }

                glBegin(GL_QUADS);
                {
                    glVertex3f(A.x,A.y,A.z);
                    glVertex3f(B.x,B.y,B.z);
                    glVertex3f(C.x,C.y,C.z);
                    glVertex3f(D.x,D.y,D.z);
                }
                glEnd();
            }
        }

        glPopMatrix();
    }
    double intersect(Ray ray, double * color, int level)
    {
        double down=DOT(Vector(0,0,1),ray.Rd);
        if(isSame(down,0))
            return -1;
        double up=-DOT(Vector(0,0,1),ray.Ro);

        double t=up/down;

        Point intersectionPoint=ray.Ro+ray.Rd*t;

        if(!(intersectionPoint.x>=-floorWidth/2&&intersectionPoint.x<=floorWidth/2&&intersectionPoint.y>=-floorWidth/2&&intersectionPoint.y<=floorWidth/2))
            return -1;

        int i=floor((intersectionPoint.x+floorWidth/2)/tileWidth);
        int j=floor((intersectionPoint.y+floorWidth/2)/tileWidth);

        assert(level==0);

        if((i+j)%2==0)
        {
            color[0]=color[1]=color[2]=1;
        }
        else
        {
            color[0]=color[1]=color[2]=0;
        }
        return t;
    }
};


