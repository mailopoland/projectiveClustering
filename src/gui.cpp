#include "gui.hpp"
#include <cmath>
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
void WindowGUI::toClusterImg(){
    clusterBut->setEnabled( false );

    qDebug() << "toClusterImg slot";
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
