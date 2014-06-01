#include "gui.hpp"

int main(int argc, char *argv[ ]){
    QApplication app( argc, argv );
    
    WindowGUI mainWindow;
    mainWindow.show();
    
    return app.exec();
}
