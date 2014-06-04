#ifndef __GUI_WINDOW_HPP__
#define __GUI_WINDOW_HPP__

#include <QtGui>
#include <QtWidgets>

class WindowGUI : public QMainWindow{
    Q_OBJECT

public:
    WindowGUI( QWidget *parent = 0 );
    ~WindowGUI();


private slots:
    void loadImg();
    void toBinImg();
    void toClusterImg();
    //use them in future, now very simplified
    //void open();
    //void save();
    //void quit();


private:
    //******WIDGETY******//
    QImage originImg;
    QImage binImg;

    QWidget *mainWid{ new QWidget };

    QPushButton *openBut { new QPushButton( tr( "&Open" ) ) };
    QPushButton *quitBut{ new QPushButton( tr( "&Quit" ) ) };
    QPushButton *binarizeBut{ new QPushButton( tr( "&Binarize" ) ) };
    QPushButton *clusterBut{ new QPushButton( tr( "&Cluster" ) ) };


    QVBoxLayout *mainLay { new QVBoxLayout };
    QLabel *imgLab { new QLabel( tr("Your Image to Process" ) ) };

    
    //@TODO add window QMenu and QActions
    //QAction *openAct;
    //QAction *saveAct;
    //QAction *exitAct;

    //QMenu *fileMenu;
};

#endif
