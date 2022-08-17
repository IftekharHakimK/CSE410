class Matrix33
{
public:

    double v[3][3];
    Matrix33(double * _v) // size 9
    {
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {
                v[i][j]=_v[i*3+j];
            }
        }
    }
    double determinant()
    {
        return v[0][0]*(v[1][1]*v[2][2]-v[1][2]*v[2][1])-v[0][1]*(v[1][0]*v[2][2]-v[1][2]*v[2][0])+v[0][2]*(v[1][0]*v[2][1]-v[1][1]*v[2][0]);
    }
};
