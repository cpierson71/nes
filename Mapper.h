#ifndef MAPPER_HXX
#define MAPPER_HXX

#include <vector>
#include <stdint.h>
#include <iostream>
#include "Memory.h"

namespace NesMapper {

struct MapperInfo {
    uint32_t mapperNum;
    uint32_t numPrgRomBanks;
    uint32_t numChrRomBanks;
    bool trainerPresent;
};

static const uint32_t prgRomSize = 0x4000;
static const uint32_t prgRomStartingAddress = 0x8000;
static const uint32_t prgRomUpperBankOffset = 0x4000;

class Mapper {

public:
    Mapper() : mapperInfo{}
    {}

    MapperInfo getMapperInfo();

    void setMapperInfo(MapperInfo info);

    void initialize(std::vector<uint8_t> &cartridgeData, Memory &memory);

private:
    MapperInfo mapperInfo;

};

}

#endif