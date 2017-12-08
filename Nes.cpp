#include "Memory.h"
#include "Cpu.h"
#include "NesReader.h"

#include <iostream>

int main()
{
    NesCpu::Cpu cpu{};
    Memory memory{};
    NesReader nesReader{};

    nesReader.setFilename("Contra (USA).nes");
    nesReader.open();

    NesCpu::opCode opcode;
    opcode = nesReader.readOperation();

    std::cout << opcode << std::endl;

    return 0;
}

