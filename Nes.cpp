#include "Memory.h"
#include "Cpu.h"

#include <iostream>

int main()
{
    Cpu cpu{};
    Memory memory{};


    memory.write(50, -5);
    std::cout << static_cast<int>(memory.read(50)) << std::endl;

    cpu.ADC(memory, 50);
    std::cout << cpu.flagString() << std::endl;

    for(int i = 0; i < 65535; ++i)
    {
        cpu.INC(memory, i);
    }

    std::cout << static_cast<int>(memory.read(0x10FE)) << std::endl;

    int8_t*  addr_p = memory.getAddress(10);
    std::cout << static_cast<int>(*addr_p) << std::endl;


    return 0;
}

