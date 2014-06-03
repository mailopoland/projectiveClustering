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

    //use them in future, now very simplified
    //void open();
    //void save();
    //void quit();


private:
    //******WIDGETY******//
    QImage *originImg;

    QWidget *mainWid;

    QPushButton *openImgBut;
    QPushButton *quitBut;
    
    QVBoxLayout *mainLay;
    QLabel *imgLab;

    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;

    QMenu *fileMenu;
};

#endif
