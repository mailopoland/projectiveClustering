#ifndef __IMAGE_CLUSTER_HPP__
#define __IMAGE_CLUSTER_HPP__
#include <QImageReader>

class ImageCluster{
public:
    void open();



private:
    QImageReader *reader;
    QImageWriter *writer;
};
