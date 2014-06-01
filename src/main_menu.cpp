#include "main_menu.hpp"
//#include "ui_mainmenu.h"
//#include "calculator.h"

#include <QDebug>

MainMenu::MainMenu(QWidget *parent) :
    QDialog(parent)
{
    qDebug() << "Konstruktor main_menu";
    //ui->setupUi(this);
    //connect( ui->pushButtonExit, SIGNAL(clicked()), SLOT(close()) );
    //connect( ui->pushButtonCalc, SIGNAL(clicked()), SLOT(openCalc()) );
}

MainMenu::~MainMenu()
{
    qDebug() << "Destruktor ~main_menu";
    //delete ui;
}
void MainMenu::openCalc()
{
    //calc = new calculator(  );
    //this->close();
    //calc->exec();
    //delete calc;
    //this->show();
}
