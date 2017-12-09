#include <iostream>
#include "Console.h"

void Console::initialize()
{
    nesReader.setFilename("Contra (USA).nes");
    nesReader.initialize(mapper);
    NesMapper::MapperInfo mapperInfo = mapper.getMapperInfo();
    std::vector<uint8_t> cartridgeData = nesReader.getCartridgeData();

    std::cout << "data: " << cartridgeData[16] << '\n';

    mapper.initialize(cartridgeData, memory);

    cpu.reset(memory);


}