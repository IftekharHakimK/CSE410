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
//        dbg(t);
        assert(level==0);
        color[0]=this->color[0];
        color[1]=this->color[1];
        color[2]=this->color[2];
//        cout<<endl;
        return t;

    }
};
