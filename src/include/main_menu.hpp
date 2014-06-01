#ifndef __MAIN_MENU_HPP__
#define __MAIN_MENU_HPP__

#include <QDialog>
//#include "calculator.h"

namespace Ui {
    class MainMenu;
}

class MainMenu : public QDialog{
    Q_OBJECT
    
public:
    explicit MainMenu(QWidget *parent = 0);
    ~MainMenu();
    
private slots:
    void openCalc();

private:
    //Ui::MainMenu *ui;
    //calculator *calc;
};

#endif
