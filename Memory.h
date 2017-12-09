#ifndef MEMORY_HXX
#define MEMORY_HXX

#include <stdint.h>

class Memory {

public:
    Memory() : data{}
    {}
       
    uint8_t read(uint16_t address);

    void write(uint16_t address, int8_t value);

    uint8_t* getAddress(uint16_t address);

    void setBit(uint16_t address, uint8_t bitNum, bool set);

private:
    uint8_t data[65535]; // 16-bit address

};

#endif