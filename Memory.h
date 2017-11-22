#ifndef MEMORY_HXX
#define MEMORY_HXX
#include <stdint.h>

class Memory {

public:
    Memory() : RAM{}
    {}
       
    int8_t read(uint16_t address);

    void write(uint16_t address, int8_t value);

    int8_t* getAddress(uint16_t address);

private:
    int8_t RAM[65535]; // 16-bit address

};

#endif