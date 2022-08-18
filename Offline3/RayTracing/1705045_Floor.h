extern vector<PointLight>pointLights;
extern vector<SpotLight>spotLights;
extern vector<Object*>objects;
extern int recursionLevel;

class Floor: public Object
{
public:
    double floorWidth;
    double tileWidth;

    Floor(double _floorWidth,double _tileWidth)
    {
        floorWidth=_floorWidth;
        tileWidth=_tileWidth;
        ambient=0.4;
        diffuse=0.2;
        specular=0.2;
        reflection=0.2;
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

        Vector normal=Vector(0,0,1);
        if(DOT(normal,ray.Rd*-1)<0)
        {
            normal=normal*-1;
        }

        double down=DOT(normal,ray.Rd);
        if(isSame(down,0))
            return -1;
        double up=-DOT(normal,ray.Ro);

        double t=up/down;

        Point intersectionPoint=ray.Ro+ray.Rd*t;

        if(!(intersectionPoint.x>=-floorWidth/2&&intersectionPoint.x<=floorWidth/2&&intersectionPoint.y>=-floorWidth/2&&intersectionPoint.y<=floorWidth/2))
            return -1;

        if(t<0||level==0)
            return t;

        int i=floor((intersectionPoint.x+floorWidth/2)/tileWidth);
        int j=floor((intersectionPoint.y+floorWidth/2)/tileWidth);

        double intersectionPointColor[3]= {0,0,0};

        if((i+j)%2==0)
        {
            intersectionPointColor[0]=1;
            intersectionPointColor[1]=1;
            intersectionPointColor[2]=1;
        }
        else
        {
            intersectionPointColor[0]=0;
            intersectionPointColor[1]=0;
            intersectionPointColor[2]=0;
        }

        color[0]=intersectionPointColor[0]*ambient;
        color[1]=intersectionPointColor[1]*ambient;
        color[2]=intersectionPointColor[2]*ambient;

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
//            cout<<"here"<<endl;
        }
//
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


