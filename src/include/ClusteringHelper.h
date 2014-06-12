#ifndef CLUSTERINGHELPER_H
#define CLUSTERINGHELPER_H
#include <list>

using namespace std;

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
    protected:
    private:
};

#endif // CLUSTERINGHELPER_H
