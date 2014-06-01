#include "gui.hpp"

WindowGUI::WindowGUI( QWidget *parent )
    : QMainWindow( parent ){

qDebug() << "Konstruktor Window GUI \n";

    quitBut = new QPushButton( tr( "Quit" ) );
    openImgBut = new QPushButton( tr("Open" ) );

    connect( quitBut, SIGNAL( clicked() ), this, SLOT( close() ) );
    connect( openImgBut, SIGNAL( clicked() ), this, SLOT( loadImg() ) );
     
    imgLab = new QLabel( tr("Your Image") );
    
    mainLay = new QVBoxLayout;
    mainLay->addWidget( imgLab );
    
    mainLay->addWidget( openImgBut );
    mainLay->addWidget( quitBut );  
    
    mainWin = new QWidget;
    mainWin->setLayout( mainLay );

    this->setCentralWidget( mainWin );
}
WindowGUI::~WindowGUI(){
qDebug() << "Destruktor Window GUI \n";
    delete quitBut;
    delete openImgBut;
    delete mainLay;
    delete mainWin;
}

void WindowGUI::open(){}
void WindowGUI::save(){}
void WindowGUI::loadImg(){}
