#include <iostream>
#include "Console.h"

void Console::initialize()
{
    nesReader.setFilename("Contra (USA).nes");
    nesReader.initialize(mapper);
    NesMapper::MapperInfo mapperInfo = mapper.getMapperInfo();
    NesReader::uint8Vec* cartridgeData = nesReader.getCartridgeData();
    mapper.initialize(*cartridgeData, memory);
    cpu.reset(memory);

    std::cout << "end\n";
}