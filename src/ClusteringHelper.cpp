#include "ClusteringHelper.h"
#include <stdlib.h> /* srand, rand */
#include <time.h> /* time */
#include <algorithm> //sort
#include <cmath>

using namespace std;

void ClusteringHelper::findRandPoints( const unsigned int k , list<point*> * curList , const unsigned int curSize, point * randPoints[] ){
    unsigned int randInd[k];
    srand(time(NULL)); //initialize random seed:
    //looking for indexes of random points:
    for( unsigned int kIt = 0; kIt < k ; ){
        unsigned int randTemp = rand() % curSize;
        bool isExInRand = false;
        //checking that randTemp isn't exsisted in rand
        for( unsigned int rCheck = 0 ; rCheck < kIt ; rCheck++ ){
            if(randInd[rCheck] == randTemp ) isExInRand = true;
        }
        if( !isExInRand ){
            randInd[kIt] = randTemp;
            kIt++; //only there kIt += 1
        }
    }
    sort (randInd, randInd + k); //for easier find correct value in list
    /*
    //for debug/test:
    for( unsigned int i = 0; i < k ; i++){
        qDebug() << "Random index number:" << i;
        qDebug() << "x:" << randInd[i];
    }
    */
    //looking for these random points
    unsigned int indexSearch = 0;
    unsigned int randIt = 0;

    for( list<point*>::iterator listIt = curList->begin(); randIt < k ; ){
        if( indexSearch == randInd[randIt] )
            randPoints[ randIt++ ] = *listIt;
        listIt++;
        indexSearch++;
    }
    /*
    //for debug/test:
    qDebug() << "In ClusteringHelper:";
    for( int i = 0; i < k ; i++){
        qDebug() << "Random point number: " << i;
        qDebug() << "x:" << randPoints[i]->x;
        qDebug() << "y:" << randPoints[i]->y;
    }
    */
}
double ClusteringHelper::pointsDestNormal( point * a , point * b){ //||a - b||^2
    return pow( pow( pow( (double)( a->x - b->x ) , 2. ) - pow( (double)( a->y - b->y ) , 2. ), 1/2 ), 2);
}
