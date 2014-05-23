#ifndef __PROJECTIVE_CLUSTERING__HPP__
#define __PROJECTIVE_CLUSTERING__HPP__

#include <string>
#include <fstream>
class Image{
public:
    void openFile( const std::string& nameFile );
    void closeFile();
private:
    std::fstream imageFile;
};
#endif

