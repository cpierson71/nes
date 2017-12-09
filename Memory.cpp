#include <iostream>
#include "Memory.h"


int8_t Memory::read(uint16_t address)
{
    return data[address];
}

void Memory::write(uint16_t address, int8_t value)
{
    data[address] = value;
}

int8_t* Memory::getAddress(uint16_t address)
{
    return &data[address];
}

void Memory::setBit(uint16_t address, uint8_t bitNum, bool set)
{
    if (bitNum < 8)
    {
        uint8_t bitMask = 1;
        bitMask << bitNum;
        if (set)
        {
            data[address] |= bitMask;
        }
        else
        {
            bitMask = ~bitMask;
            data[address] &= bitMask;
        }
    }
    else
    {
        std::cout << "Error setting bit in memory. bitNum > 7 \n";
    }

}