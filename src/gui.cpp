#include "gui.hpp"
#include "ClusteringHelper.cpp"
#include <cmath>
#include <list>
#include <iostream>

using namespace std;



WindowGUI::WindowGUI( QWidget *parent )
    : QMainWindow( parent ){
    //@TODO
    //try to consider better GUI for showing and manipulating img
    //e.g put image into scrollbar or sth, add window QMenu + QActions


    //thanks to c++11 new feature - default initialization by member ( by new )
    //create objects in declaration, so no need to do that - simpler construcor

    //quitBut = new QPushButton( tr( "Quit" ) );
    //openBut = new QPushButton( tr("Open" ) );
    //binarizeBut = new QPushButton( tr("Binarize") );
    //clusterBut = new QPushButton( tr("Cluster") );
    //imgLab = new QLabel( tr("Your Image to Process") );
    //mainWid = new QWidget;
    //mainLay = new QVBoxLayout;

    qDebug() << "Conctructor WindowGUI \n";
    clusterBut->setEnabled( false );
    binarizeBut->setEnabled( false );

    connect( quitBut, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( openBut, SIGNAL( clicked() ), this, SLOT( loadImg() ) );
    connect( binarizeBut, SIGNAL( clicked() ), this, SLOT( toBinImg() ));
    connect( clusterBut, SIGNAL( clicked() ), this, SLOT( toClusterImg() ));

    imgLab->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    imgLab->setScaledContents( true );

    mainLay->addWidget( imgLab );
    mainLay->addWidget( openBut );
    mainLay->addWidget( binarizeBut );
    mainLay->addWidget( clusterBut );
    mainLay->addWidget( quitBut );

    mainWid->setLayout( mainLay );

    this->setCentralWidget( mainWid );

    resize( 600, 500 );
    setWindowTitle( tr("Projective Clustering" ) );
}
WindowGUI::~WindowGUI(){
    qDebug() << "Destructor WindowGUI \n";
    delete quitBut;
    delete openBut;
    delete binarizeBut;
    delete clusterBut;
    delete mainLay;
    delete mainWid;
}


//*************------------SLOTS------------************
void WindowGUI::toBinImg(){
    //@TODO consider strategy pattern for binarization and making histogram
    //FOR NOW - create grayscale img from origin, make simple histogram, use
    //simplified otsu method and show img, everything in one method :(

    binarizeBut->setEnabled( false );
    clusterBut->setEnabled( true );

    qDebug() << "toBinImg slot";

   //copy img, change to grayscale and make histogram, consider 8bit format
    QImage grayImg { originImg.convertToFormat( QImage::Format_RGB32 ) };
    if ( grayImg.isNull() ){
        QMessageBox::information( this, tr("Projective Clustering"),
                                    tr("cannot load"));
        return;
    }
    int gray{}, width{ grayImg.width() }, height { grayImg.height() };
    long histogram[256] = {};
    for ( int i = 0; i < width; ++i){
        for ( int j = 0; j < height; ++j ){
            gray = qGray( grayImg.pixel( i, j ) ) ;
            histogram[ gray ]++;
            //grayImg.setPixel( i, j, qRgb( gray, gray, gray ) );
        }
    }
    //simple otsu method - use created grayscale histogram to compute threshold
    //theoretically need total pixels and histogram, return threshold
    double sum{}, sumB{}, wB{}, wF{}, mB{}, mF{}, max{}, between{},
           threshold1{}, threshold2{}, results{};
    long total{ ( width * height ) };
    for ( int i = 1; i < 256; ++i )
        sum += i * histogram[i];
    for ( int i = 0; i < 256; ++i) {
        wB += histogram[i];
        if (wB == 0)
            continue;
            wF = total - wB;
        if (wF == 0)
            break;
        sumB += i * histogram[i];
        mB = sumB / wB;
        mF = (sum - sumB) / wF;
        between = wB * wF * qPow(mB - mF, 2);
        if ( between >= max ) {
            threshold1 = i;
            if ( between > max ) {
                threshold2 = i;
            }
            max = between;
        }
    }
    results = ( threshold1 + threshold2 ) / 2.0;

    //now change all pixel below results to white and above to black
    for ( int i = 0; i < width; ++i ){
        for ( int j = 0; j < height; ++j ){
            double avg = static_cast<double>( qGray( originImg.pixel( i, j ) ));
            if( results < avg )
                grayImg.setPixel( i, j, qRgb( 0, 0, 0 ) );
            else
                grayImg.setPixel( i, j, qRgb( 255, 255, 255 ) );
        }
    }
    imgLab->setPixmap( QPixmap::fromImage( grayImg ) );
    binImg = std::move( grayImg );
}
void WindowGUI::loadImg(){
    //@TODO better load, show and maybe scale img, e.g look for QImageReader
    //and consider file formats e.g  setting after QDir path in getOpenFileName
    //"Images (*.png *.xpm *.jpg)" if you want validate images after extensions
    qDebug() << "loadImg slot";

    //directory including matches images is in dir where our .exe file is
    QString imgName = QFileDialog::getOpenFileName( this, tr("Open Image"),
                                QCoreApplication::applicationDirPath()+"/img/");
    if ( !imgName.isEmpty() ){
        QImage img( imgName );
        if ( img.isNull() ){
            QMessageBox::information( this, tr("Projective Clustering"),
                    tr("cannot load %1").arg( imgName ));
            return;
        }
        clusterBut->setEnabled( false );
        binarizeBut->setEnabled( true );
        imgLab->setPixmap( QPixmap::fromImage( img ) );
        originImg = std::move( img );
    }
}

void WindowGUI::toClusterImg(){
    clusterBut->setEnabled( false );

    qDebug() << "toClusterImg slot";

    qDebug() << "Data preparing...";
    //---prepare data---
    const unsigned int k = 2;
    const double w[] = {1/3,1/3,1/3};
    list<point*> clusters[k];
    list<point*> whitePoints;
    list<point*> blackPoints;
    list<point*> * curList; //blackPoints or whitePoints depend on their amount of elements
    unsigned int whiteSize = 0;
    unsigned int blackSize = 0;
    unsigned int curSize;

    unsigned int imgH = (unsigned int)binImg.height();
    unsigned int imgW = (unsigned int)binImg.width();
    qDebug() << "Picture height: " << imgH << "px";
    qDebug() << "Picture width: " << imgW << "px";
    for( unsigned int h = 0 ; h < imgH ; h++ ){
        for( unsigned int w = 0 ; w < imgW ; w++){
            if(qGray(binImg.pixel( w , h ) == 0 )){ //black
                blackPoints.push_front( new point(h,w) );
                blackSize++;
            }
            else{ // qGray(binImg.pixel( w , h ) == 255 | white
                whitePoints.push_front( new point(h,w) );
                whiteSize++;
            }
        }
    }
    //we expext that contect is smaller than background
    //select smaller curList:
    if( whiteSize > blackSize ){
        curList = &whitePoints;
        curSize = whiteSize;
    }
    else{
        curList = &blackPoints;
        curSize = blackSize;
    }
    qDebug() << "Finished: Data preparing";
    //---start alghoritm---
    qDebug() << "Start alghoritm";
    //@begin standard clastering steps:
    //(1) find k random points
    point * randPoints[k]; //randPoints for each clusters
    qDebug() << "Finding random points... ";
    ClusteringHelper::findRandPoints( k , curList, curSize, randPoints);
    /*
    //for debug/test:
    for( int i = 0; i < k ; i++){
        qDebug() << "Random point number: " << i;
        qDebug() << "x:" << randPoints[i]->x;
        qDebug() << "y:" << randPoints[i]->y;
    }
    */
    qDebug() << "Finished: Finding random points";
    //(2) obtain correct cluster for all points
    qDebug() << "Match points to the nearest cluster...";
    for( list<point*>::iterator listIt = curList->begin(); listIt != curList->end() ; listIt++){
        double min = 1.79769e+308; //max value of double
        unsigned int clusterIndex;
        //check dist between all clusters selected point
        for( unsigned int clusterIt ; clusterIt < k ; clusterIt++ ){
            double actCheckVal = ClusteringHelper::pointsDestNormal( *listIt, randPoints[clusterIt]);
            if( actCheckVal < min ){
                min = actCheckVal;
                clusterIndex = clusterIt;
            }
        }
        //save point to correct cluster
        clusters[clusterIndex].push_front(*listIt);
    }
    qDebug() << "Finished: Match points to the nearest cluster";
    /*
    //for debug/test:
    int clSize = 0;
    for( int i = 0; i < k ; i++){
        clSize += (int)clusters[i].size();
    }
    if( clSize == curSize ){
        qDebug() << "correct sum of size of all clusters";
    }
    else{
        qDebug() << "incorrect sum of size of all clusters!";
        qDebug() << "expected (curSize):" << curSize << " actual(clSize):" << clSize;
    }
    */

    //FIRST STEP OF PROJECTIVE PART

}
