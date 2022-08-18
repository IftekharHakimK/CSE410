extern vector<PointLight>pointLights;
extern vector<SpotLight>spotLights;
extern vector<Object*>objects;

class Sphere: public Object
{
public:

    Point center;
    double radius;

    Sphere(Point _center, double _radius)
    {
        center=_center;
        radius=_radius;
    }
    void draw()
    {

        const int stacks=45;
        const int slices=45;

        Point points[100][100];
        int i,j;
        double h,r;
        //generate points
        for(i=0; i<=stacks; i++)
        {
            h=radius*sin(((double)i/(double)stacks)*(pi/2));
            r=radius*cos(((double)i/(double)stacks)*(pi/2));
            for(j=0; j<=slices; j++)
            {
                points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
                points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
                points[i][j].z=h;
            }
        }

        glPushMatrix();
        glTranslatef(center.x,center.y,center.z);

        //draw quads using generated points
        for(i=0; i<stacks; i++)
        {
            glColor3f(color[0],color[1],color[2]);
            for(j=0; j<slices; j++)
            {
                glBegin(GL_QUADS);
                {
                    //upper hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                    //lower hemisphere
                    glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                    glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                    glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                    glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
                }
                glEnd();
            }
        }
        glPopMatrix();
    }

    double intersect(Ray ray, double * color, int level)
    {
        //origin implicitly moved to center, has no impact on value of parameter t
        ray.Ro=ray.Ro-center;

//        printPoint(ray.Ro);
//        printPoint(ray.Rd);
//        cout<<"circle: "<<radius<<endl;


        double a=1;
        double b=DOT(ray.Ro,ray.Rd)*2;
        double c=DOT(ray.Ro,ray.Ro)-radius*radius;

        double d=b*b-4*a*c;
        double t;

        if(d<0)
        {
            t=-1;
        }
        else if(isSame(d,0))
        {
            t=-b/(a*2);
        }
        else
        {
            d=sqrt(d);
            double t_minus=(-b-d)/(a*2);
            double t_plus=(-b+d)/(a*2);

            if(t_minus>0)
            {
                t=t_minus;
            }
            else if(t_plus>0)
            {
                t=t_plus;
            }
            else
            {
                t=-1;
            }
        }
        if(level==0||t==-1)
            return t;
        ray.Ro=ray.Ro+center;
        Point intersectionPoint=ray.Ro+ray.Rd*t;

        double intersectionPointColor[3]={0,0,0};

        intersectionPointColor[0]=this->color[0];
        intersectionPointColor[1]=this->color[1];
        intersectionPointColor[2]=this->color[2];

        color[0]=intersectionPointColor[0]*ambient;
        color[1]=intersectionPointColor[1]*ambient;
        color[2]=intersectionPointColor[2]*ambient;

        Vector normal=intersectionPoint-center;
        normal=normal/LEN(normal);

        for(PointLight pl:pointLights)
        {
            Ray L(pl.pos,intersectionPoint-pl.pos);

            double t_block=INF;
            for(auto u:objects)
            {
                double dummy_color[3]={0,0,0};
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

        for(SpotLight sl:spotLights)
        {
            Ray L(sl.pos,intersectionPoint-sl.pos);

            //check cutoff
            double angle=getAngleInDegree(sl.light_direction,L.Rd);
            if(angle>sl.cutoff_angle)
            {
                continue;
            }
            cout<<"blocked"<<endl;

            double t_block=INF;
            for(auto u:objects)
            {
                double dummy_color[3]={0,0,0};
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
            dbg(lambert);
            dbg(phong);
//            cout<<"here"<<endl;
        }

        return t;



    }
};
