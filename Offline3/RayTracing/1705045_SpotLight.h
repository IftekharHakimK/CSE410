class SpotLight
{
public:

    Point pos;
    double color[3];
    Vector light_direction;
    double cutoff_angle; // degree

    SpotLight(Point _pos, double r,double g, double b,Vector dir,double _cutoff)
    {
        pos=_pos;
        color[0]=r;
        color[1]=g;
        color[2]=b;
        light_direction=dir;
        cutoff_angle=_cutoff;
    }

};

