#include <iostream>
#include <exception>
#include "NesReader.h"

void NesReader::initialize(NesMapper::Mapper &mapper)
{
    NesMapper::MapperInfo mapperInfo;

    open();
    readCartridge();
    std::cout << "2data: " << cartridgeData[16] << '\n';
    mapperInfo = readHeader();
    mapper.setMapperInfo(mapperInfo);


}

std::vector<uint8_t> NesReader::getCartridgeData()
{
    return cartridgeData;
}

void NesReader::setFilename(std::string name)
{
    filename = name;
}

void NesReader::open()
{
    fileStream.open(filename, std::ios::binary);

    if (!fileStream.is_open())
    {
        std::cout << "Error opening file \n";
    }
}

void NesReader::readCartridge()
{
    try 
    {
        while (!fileStream.eof())
        {
            uint8_t byte = static_cast<uint8_t>(fileStream.get());
            std::cout << (int)byte << '\n';
            cartridgeData.push_back(byte);
        }
    }
    catch(std::exception &e)
    {
        std::cout << e.what() << '\n';
    }
    fileStream.close();
}

NesMapper::MapperInfo NesReader::readHeader()
{
    NesMapper::MapperInfo mapperInfo;

    // .nes file header is 16 bytes
    constexpr uint32_t headerSize = 16;
    uint8_t header[headerSize];
    for (uint32_t i = 0; i < headerSize; ++i)
    {
        header[i] = cartridgeData[i];
    }

    // Check that it is a .nes file
    const uint32_t expectedSize = 4;
    const uint8_t expectedNesHeader[]{0x4E, 0x45, 0x53, 0x1A};
    for (uint32_t i = 0; i < expectedSize; ++i)
    {
        if (header[i] != expectedNesHeader[i])
        {
            std::cout << "Invalid file format \n";
            return mapperInfo;
        }
    }

    uint8_t numPrgRomBanks = header[4];
    uint8_t numChrRomBanks = header[5];
    uint8_t flags6 = header[6];
    uint8_t flags7 = header[7];
    uint8_t prgRamSize = header[8];
    uint8_t flags9 = header[9];
    uint8_t flags10 = header[10];
    // Bytes 11-15 zero-filled

    // Read flags
    bool mirroringVertical = (flags6 & 0x01) != 0;
    bool prgRamPresent = (flags6 & 0x02) != 0;
    bool trainerPresent = (flags6 & 0x04) != 0;
    bool ignoreMirroring = (flags6 & 0x08) != 0;
    uint8_t lowerMapperNum = flags6 & 0xF0;
    lowerMapperNum >>= 4;
    bool vsUnisystem = (flags7 & 0x01) != 0;
    bool playChoice10 = (flags7 & 0x02) != 0;
    bool nes2Format = ((flags7 & 0x08) != 0) && ((flags7 & 0x04) == 0);
    uint8_t upperMapperNum = flags7 & 0xF0;
    uint8_t mapperNum = upperMapperNum | lowerMapperNum;


    bool isPal = (flags9 & 0x01) != 0;

    uint8_t tvSystem = flags10 * 0x03;
    bool prgRamPresent2 = (flags10 & 0x10) != 0;
    bool busConflicts = (flags10 & 0x20) != 0;

    if (trainerPresent)
    {
        readTrainer();
    }

    mapperInfo.mapperNum = mapperNum;
    mapperInfo.numPrgRomBanks = numPrgRomBanks;
    mapperInfo.numChrRomBanks = numChrRomBanks;
    mapperInfo.trainerPresent = trainerPresent;
    return mapperInfo;
}

void NesReader::readTrainer()
{
    // TODO: Do anything with trainer data?

}

