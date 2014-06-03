#include "gui.hpp"

WindowGUI::WindowGUI( QWidget *parent )
    : QMainWindow( parent ){
    //@TODO try to consider better GUI for showing and manipulating img
    //e.g put image into scrollbar or sth
    qDebug() << "Constructor WindowGUI \n";

    quitBut = new QPushButton( tr( "Quit" ) );
    openImgBut = new QPushButton( tr("Open" ) );

    connect( quitBut, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( openImgBut, SIGNAL( clicked() ), this, SLOT( loadImg() ) );
     
    imgLab = new QLabel( tr("Your Image to Cluster") );
    imgLab->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    imgLab->setScaledContents( true );

    mainLay = new QVBoxLayout;
    mainLay->addWidget( imgLab );
    mainLay->addWidget( openImgBut );
    mainLay->addWidget( quitBut );  
    
    mainWid = new QWidget;
    mainWid->setLayout( mainLay );

    this->setCentralWidget( mainWid );

    resize( 600, 500 );
    setWindowTitle( tr("Projective Clustering" ) );
}
WindowGUI::~WindowGUI(){
qDebug() << "Destructor WindowGUI \n";
    delete quitBut;
    delete openImgBut;
    delete mainLay;
    delete mainWid;
}

void WindowGUI::loadImg(){
    //@TODO better load, show and maybe scale img, e.g look for QImageReader
    //and consider file formats e.g  setting after QDir path in getOpenFileName
    //"Images (*.png *.xpm *.jpg)" if you want validate images after extensions 
    qDebug() << "loadImg slot";

    //directory including match images is in dir where our .exe file is 
    QString imgName = QFileDialog::getOpenFileName( this, tr("Open Image"),
                                QCoreApplication::applicationDirPath()+"/img/");
    if ( !imgName.isEmpty() ){
        QImage img( imgName );
        if ( img.isNull() ){
            QMessageBox::information( this, tr("Projective Clustering"),
                    tr("cannot load %1").arg( imgName ));
            return;
        }
        imgLab->setPixmap( QPixmap::fromImage( img ) );
    }
}
