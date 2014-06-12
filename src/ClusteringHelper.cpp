#include "ClusteringHelper.h"
#include <stdlib.h> /* srand, rand */
#include <time.h> /* time */
#include <algorithm> //sort
#include <cmath>
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

using namespace Eigen;


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
    double a2 = pow( ( (double)(a->x) - (double)(b->x) ) , 2. );
    //qDebug() << "a2=" << a2;
    double b2 = pow( ( (double)(a->y) - (double)(b->y) ) , 2. );
    //qDebug() << "b2=" << b2;
    double a2PLUSb2 = a2 + b2;
    //qDebug() << "a2PLUSb22=" << a2PLUSb2;
    double sqr = sqrt(  a2PLUSb2  );
    //qDebug() << "sqr=" << sqr;
    //qDebug() << "result ^2" << pow( sqr, 2);
    return pow( sqr, 2);
}

Matrix<double,2,1> ClusteringHelper::getAverage( list<point*> cluster){ //get average of cluster
    Matrix<double,2,1> result;
    result(0, 0) = 0; //x
    result(1, 0) = 0; //y
    double n = 0;
    for( list<point*>::iterator listIt = cluster.begin(); listIt != cluster.end() ; listIt++ ){
        result(0, 0) += (*listIt)->x;
        result(1, 0) += (*listIt)->y;
        n++;
    }
    if( n != 0 ){
        result(0,0) = result(0,0) / n;
        result(1,0) = result(1,0) / n;
    }
    double t = result(0,0);
    double t2 = result(1,0);
    return result;
}

Matrix<double,2,2> ClusteringHelper::getCovarianceMatrix( list<point*> cluster, Matrix<double,2,1> avg ){
    Matrix<double,2,2> result;
    result(0, 0) = 0;
    result(1, 0) = 0;
    result(0, 1) = 0;
    result(1, 1) = 0;
    double n = 0;
    for( list<point*>::iterator listIt = cluster.begin(); listIt != cluster.end() ; listIt++ ){
        Matrix<double,2,1> actualPoint;
        actualPoint(0, 0) = (*listIt)->x;
        actualPoint(1, 0) = (*listIt)->y;
        result += (actualPoint - avg) * (actualPoint - avg).transpose();
        n++;
    }
    result = result / n;
    return result;
}

Matrix<double,2,2> ClusteringHelper::getEigenvectors( Matrix<double,2,2> covMat ){
    Matrix<double,2,2> result;
    EigenSolver<Matrix<double,2,2>> es(covMat);
    auto  vectors = es.eigenvectors();
    for(int i = 0; i < covMat.rows() ; i++ ){ //covMat.rows() should = 2
        result(i,0) = real(vectors.col(i)[0] );
        result(1,i) = real(vectors.col(i)[1] );
    }
    return result;
}



