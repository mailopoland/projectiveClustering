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

void ClusteringHelper::findRandPoints( const int k , list<point*> * curList , const int curSize, point * randPoints[] ){
    double randInd[k];
    srand(time(NULL)); //initialize random seed:
    //looking for indexes of random points:
    for( int kIt = 0; kIt < k ; ){
        double randTemp = rand() % curSize;
        bool isExInRand = false;
        //checking that randTemp isn't exsisted in rand
        for( int rCheck = 0 ; rCheck < kIt ; rCheck++ ){
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
    for( double i = 0; i < k ; i++){
        qDebug() << "Random index number:" << i;
        qDebug() << "x:" << randInd[i];
    }
    */
    //looking for these random points
    int indexSearch = 0;
    int randIt = 0;

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
    double a2 = pow( ( (a->x) - (b->x) ) , 2. );
    //qDebug() << "a2=" << a2;
    double b2 = pow( ( (a->y) - (b->y) ) , 2. );
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
double ClusteringHelper::mathFunc( point * a, point * b){ //<a,b>^2
    return pow( ( a->x * b->y ) + (a->y * b->x) , 2 );
}

double ClusteringHelper::dist( const double w[], point * pointCur, Matrix<double,2,1> avg, Matrix<double,2,2> vectors){
    double result = 0;
    //point avgP ;
   // qDebug() << "start";
    double a1 = avg[0,0];
   // qDebug() << "avg1=" << a1;
    double a2 = avg[0,1];
   // qDebug() << "avg2=" << a2;
    point * avgP = new point( a1, a2 );
    double v11 = vectors(0,0);
   // qDebug() << "v11=" << v11 ;
    double v12 = vectors(0,1);
  //  qDebug() << "v12=" << v12 ;
    double v21 = vectors(1,0);
  //  qDebug() << "v21=" << v21 ;
    double v22 = vectors(1,1);
   // qDebug() << "v22=" << v11 ;
    point * v1 = new point( v11, v12 );
    point * v2 = new point( v21, v22 );
   // qDebug() << "p2";
    point * xMINUSm = new point ( pointCur->x - avgP->x , pointCur->y - avgP->y );
    double helpDist = ClusteringHelper::pointsDestNormal( pointCur , avgP ); // ||point-avgP - avgP||^2
    double helpF1 = mathFunc( xMINUSm, v1); //<point-avgP,v1>^2
    double helpF2 = mathFunc( xMINUSm, v2); //<point-avgP,v2>^2
    result = w[0] * helpDist + w[1] * (helpDist - helpF1) + w[2] * (helpDist - helpF1 - helpF2);
  //  qDebug() << "pe last";
    delete v1;
    delete v2;
    delete avgP;
    delete xMINUSm;

    return result;
}

void ClusteringHelper::test(){ //for me not neccessery to anything
    point * a = new point(1,2);
    delete a;
}



