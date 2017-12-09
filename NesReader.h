#ifndef NESREADER_HXX
#define NESREADER_HXX

#include <fstream>
#include <string>
#include <vector>
#include <stdint.h>
#include <memory>
#include "Mapper.h"

class NesReader {

public:
    typedef std::vector<uint8_t> uint8Vec;

    NesReader() : filename{}
        , fileStream{}
        , fileSize{}
        , cartridgeData{}
    {}

    ~NesReader()
    {
        if (fileStream.is_open())
        {
            fileStream.close();
        }
    }

    void initialize(NesMapper::Mapper &mapper);

    uint8Vec* getCartridgeData();

    void setFilename(std::string name);

    void open();

    void readCartridge();

    NesMapper::MapperInfo readHeader();

    void readTrainer();
    

private:
    std::string filename;
    std::ifstream fileStream;
    size_t fileSize;
    uint8Vec cartridgeData;
};

#endif