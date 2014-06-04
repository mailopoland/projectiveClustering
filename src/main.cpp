#include "gui.hpp"

int main(int argc, char *argv[ ]){
    QApplication app( argc, argv );
    //i'm not sure is it even correct or proper
    try{
        WindowGUI mainWindow;
        mainWindow.show();
        app.exec();
    }catch( ... ){
        qDebug() << "unexpected exceptions in main.cpp";
    }
    return 0;
}
