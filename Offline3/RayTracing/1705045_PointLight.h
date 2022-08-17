class PointLight
{
public:

    Point pos;
    double color[3];

    PointLight(Point _pos, double r, double g, double b)
    {
        pos=_pos;
        color[0]=r;
        color[1]=g;
        color[2]=b;
    }

};
