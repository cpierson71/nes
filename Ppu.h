#ifndef PPU_HXX
#define PPU_HXX

#include <stdint.h>

namespace NesPpu
{

enum SpriteType : int8_t
{
    _8x8,
    _8x16
};

static const uint16_t ppuControlRegister1 = 0x2000;
static const uint16_t ppuStatusRegister = 0x2002;

class Ppu {

public:

    Ppu() : vram{}
        , spriteType{}
    {}
       
    int8_t read(uint16_t address);

    void write(uint16_t address, int8_t value);

    int8_t* getAddress(uint16_t address);

private:
    int8_t vram[65535]; // 16-bit address
    SpriteType spriteType;
};

}

#endif