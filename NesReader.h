#ifndef NESREADER_HXX
#define NESREADER_HXX

#include <fstream>
#include <string>
#include "Cpu.h"

class NesReader {

public:
    NesReader() : filename{}
        , fileStream{}
    {}

    void setFilename(std::string name);

    void open();

    NesCpu::opCode readOperation();

private:
    std::string filename;
    std::ifstream fileStream;

};

#endif