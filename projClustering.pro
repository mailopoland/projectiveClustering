CONFIG += qt unix
QT += gui widgets core

# more safe choice is add here c++11 flag
QMAKE_CXXFLAGS += -std=c++11
unix {
    TEMPLATE = app
    DESTDIR = ./bin
    INCLUDEPATH += src/include
    HEADERS += src/include/gui.hpp
    SOURCES += src/main.cpp src/gui.cpp
}
