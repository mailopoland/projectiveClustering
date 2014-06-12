#ifndef CLUSTERINGHELPER_H
#define CLUSTERINGHELPER_H
#include <list>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

using namespace std;
using namespace Eigen;

class point{
    public:
        point( unsigned int _x, unsigned int _y ){
            x = _x;
            y = _y;
        }
        unsigned int x;
        unsigned int y;
};

class ClusteringHelper
{
    public:
        static void findRandPoints( const unsigned int k , list<point*> * curList , const unsigned int curSize, point * randPoints[] );
        static double pointsDestNormal( point * a , point * b);
        static Matrix<double,2,1> getAverage( list<point*> cluster); //get average of cluster
        static Matrix<double,2,2> getCovarianceMatrix( list<point*> cluster, Matrix<double,2,1> avg );
        static Matrix<double,2,2> getEigenvectors( Matrix<double,2,2> covMat );
    protected:
    private:
};

#endif // CLUSTERINGHELPER_H
