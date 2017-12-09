#ifndef NESREADER_HXX
#define NESREADER_HXX

#include <fstream>
#include <string>
#include <vector>
#include <stdint.h>
#include "Mapper.h"

class NesReader {

public:
    NesReader() : filename{}
        , fileStream{}
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

    std::vector<uint8_t> getCartridgeData();

    void setFilename(std::string name);

    void open();

    void readCartridge();

    NesMapper::MapperInfo readHeader();

    void readTrainer();

    

private:
    std::string filename;
    std::ifstream fileStream;
    std::vector<uint8_t> cartridgeData;
};

#endif