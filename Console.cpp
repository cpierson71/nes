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


    cpu.setupOpcodes();
    cpu.opcodeInfoArray[0].operation(cpu,memory,0);

    std::cout << "end\n";
}