class Floor: public Object
{
public:
    Point ref_point;
    double length;

    Floor(double floorWidth,double tileWidth)
    {
        ref_point=Point(-floorWidth/2,-floorWidth/2,0);
        length=tileWidth;
    }
    void draw()
    {

    }
};

