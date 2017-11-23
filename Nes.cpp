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

    uint8_t x = 128;
    int8_t* x_p = reinterpret_cast<int8_t*>(&x);
    std::cout << static_cast<int>(x) << " : " << static_cast<int>(*x_p) << std::endl;

    return 0;
}

