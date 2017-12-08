#include <iostream>
#include "NesReader.h"

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

NesCpu::opCode NesReader::readOperation()
{
    NesCpu::opCode opcode;
    if (!fileStream.eof())
    {
        opcode = static_cast<NesCpu::opCode>(fileStream.get());
    }

    return opcode;
}