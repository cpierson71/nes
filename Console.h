#ifndef CONSOLE_HXX
#define CONSOLE_HXX

#include "Cpu.h"
#include "Memory.h"
#include "Ppu.h"
#include "NesReader.h"
#include "Mapper.h"

class Console {

public:
    Console() : cpu{}
        , memory{}
        , ppu{}
        , nesReader{}
        , mapper{}
    {}

    void initialize();


private:
    NesCpu::Cpu cpu;
    Memory memory;
    NesPpu::Ppu ppu;
    NesReader nesReader;
    NesMapper::Mapper mapper;

};

#endif