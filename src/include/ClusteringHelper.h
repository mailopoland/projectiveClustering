#ifndef CLUSTERINGHELPER_H
#define CLUSTERINGHELPER_H
#include <list>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

using namespace std;
using namespace Eigen;

class point{
    public:
        point( double _x, double _y ){
            x = _x;
            y = _y;
        }
        double x;
        double y;
};

class ClusteringHelper
{
    public:
        static void findRandPoints( const int k , list<point*> * curList , const int curSize, point * randPoints[] );
        static double pointsDestNormal( point * a , point * b);
        static Matrix<double,2,1> getAverage( list<point*> cluster); //get average of cluster
        static Matrix<double,2,2> getCovarianceMatrix( list<point*> cluster, Matrix<double,2,1> avg );
        static Matrix<double,2,2> getEigenvectors( Matrix<double,2,2> covMat );
        static double dist( const double w[], point * point, Matrix<double,2,1> avg, Matrix<double,2,2> vectors);
        static double mathFunc( point * a, point * b);
        static void test();
    protected:
    private:
};

#endif // CLUSTERINGHELPER_H
