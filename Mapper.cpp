#include "Mapper.h"
#include <iostream>

NesMapper::MapperInfo NesMapper::Mapper::getMapperInfo()
{
    return mapperInfo;
}

void NesMapper::Mapper::setMapperInfo(NesMapper::MapperInfo info)
{
    mapperInfo = info;
}

void NesMapper::Mapper::initialize(std::vector<uint8_t> &cartridgeData, Memory &memory)
{

    std::cout << "Mapper: " << mapperInfo.mapperNum << '\n';
    std::cout << "PRG-ROM bank num: " << mapperInfo.numPrgRomBanks << '\n';
    std::cout << "CHR-ROM bank num: " << mapperInfo.numChrRomBanks << '\n';
    std::cout << "trainerPresent: " << mapperInfo.trainerPresent << '\n';

    const uint32_t headerSize = 16;
    const uint32_t trainerSize = 512;
    uint32_t prgRomOffset = (mapperInfo.trainerPresent) ? headerSize + trainerSize : headerSize;

    if (mapperInfo.mapperNum == 2)
    {
        for (uint32_t i = 0; i < NesMapper::prgRomSize; ++i)
        {
            std::cout << "cartData: " << cartridgeData[prgRomOffset + i] << '\n';
            std::cout << "Mem address: " << NesMapper::prgRomStartingAddress + i << '\n';

            memory.write(NesMapper::prgRomStartingAddress + i, cartridgeData[prgRomOffset + i]);
            uint32_t lastRomBankStart = prgRomOffset + NesMapper::prgRomSize*(mapperInfo.numChrRomBanks-1);

            std::cout << "cartData: " << cartridgeData[lastRomBankStart + i] << '\n';
            std::cout << "Mem address: " << NesMapper::prgRomStartingAddress + NesMapper::prgRomUpperBankOffset + i << '\n';

            memory.write(NesMapper::prgRomStartingAddress + NesMapper::prgRomUpperBankOffset + i, cartridgeData[lastRomBankStart + i]);
        }
    }
    else if (mapperInfo.mapperNum == 3)
    {
       // TODO
    }
    else
    {
        std::cout << "Invalid mapper number\n";
    }
    
}