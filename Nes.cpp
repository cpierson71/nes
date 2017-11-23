#include "Memory.h"
#include "Cpu.h"

#include <iostream>

int main()
{
    Cpu cpu{};
    Memory memory{};


    memory.write(1, -25);
    std::cout << (int) memory.read(1) << '\n';
    cpu.LSR(memory, 1);
    std::cout << (int) memory.read(1) << '\n';

    std::cout << (int)(static_cast<int8_t>(0x80) & (int)0xFF) << std::endl;

    return 0;
}

