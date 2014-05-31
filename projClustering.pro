CONFIG += qt c++11 unix
QT += gui widgets

unix {
    TEMPLATE = app
    DESTDIR = ./bin
    INCLUDEPATH += src/include
    HEADERS += src/include/image.hpp
    SOURCES += src/main.cpp src/image.cpp
}
