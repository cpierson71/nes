#include "Memory.h"

int8_t Memory::read(uint16_t address)
{
    return RAM[address];
}

void Memory::write(uint16_t address, int8_t value)
{
    RAM[address] = value;
}

int8_t* Memory::getAddress(uint16_t address)
{
    return &RAM[address];
}