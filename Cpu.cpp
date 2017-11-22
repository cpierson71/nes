#include "Cpu.h"

std::string Cpu::flagString()
{
    std::string flagString = "C: " + std::to_string(C)
        + " Z: " + std::to_string(Z)
        + " I: " + std::to_string(I)
        + " D: " + std::to_string(D)
        + " B: " + std::to_string(B)
        + " V: " + std::to_string(V)
        + " N: " + std::to_string(N);

    return flagString;
}
    
/////////////////////////////////////
// 6502 Instructions:
/////////////////////////////////////

// Add with carry
void Cpu::ADC(Memory& mem, uint16_t address)
{
    const int8_t val = mem.read(address);
    const uint8_t prevA = A;
    A = A + val + C;
    Z = (A == 0) ? 1 : 0;
    C = (static_cast<int>(A) + static_cast<int>(val) + static_cast<int>(C) > 0xFF) ? 1 : 0;
    N = (A & 0x80 != 0) ? 1 : 0;
    // Overflow if accumulator flipped signs after addition && 
    // accumulator and memory value are same sign
    V = ((prevA^A) & 0x80 != 0 && (A^val) & 0x80 == 0) ? 1 : 0;
}

// Logical and
void Cpu::AND(Memory& mem, uint16_t address)
{
    const int8_t val = mem.read(address);
    A = A & val;
    Z = (A == 0) ? 1 : 0;
    N = (A & 0x80 != 0) ? 1 : 0;
}

// Branch on carry clear
void Cpu::BCC(uint16_t address)
{
    if(C == 0)
    {
        PC = address;
    }
}

// Branch on carry set
void Cpu::BCS(uint16_t address)
{
    if(C == 1)
    {
        PC = address;
    }
}

// Branch on equal
void Cpu::BEQ(uint16_t address)
{
    if(Z == 1)
    {
        PC = address;
    }
}

// Bit test
void Cpu::BIT(Memory& mem, uint16_t address)
{
    const int8_t val = mem.read(address);
    Z = (A & val) == 0 ? 1 : 0;
    N = val & 0x80;
    V = val & 0x40;
}

// Branch if minus
void Cpu::BMI(uint16_t address)
{
    if (N == 1)
    {
        PC = address;
    }
}

// Branch if not equal
void Cpu::BNE(uint16_t address)
{
    if (Z == 0)
    {
        PC = address;
    }
}

// Branch if plus
void Cpu::BPL(uint16_t address)
{
    if (N == 0)
    {
        PC = address;
    }
}

// Break
void Cpu::BRK(Memory& mem)
{
    // TODO

    // push PC to stack;
    // PHP();

    uint16_t* irqVec_p = reinterpret_cast<uint16_t*>(mem.getAddress(0xFFFE));
    PC = *irqVec_p;
    B = 1;
}

// Branch if overflow clear
void Cpu::BVC(uint16_t address)
{
    if (V == 0)
    {
        PC = address;
    }
}

// Branch if overflow set
void Cpu::BVS(uint16_t address)
{
    if (V == 1)
    {
        PC = address;
    }
}

// Clear carry flag
void Cpu::CLC()
{
    C = 0;
}

// Clear decimal mode
void Cpu::CLD()
{
    D = 0;
}

// Clear interrupt disable
void Cpu::CLI()
{
    I = 0;
}

// Clear overflow flag
void Cpu::CLV()
{
    V = 0;
}

// Compare accumulator
void Cpu::CMP(Memory& mem, uint16_t address)
{
    const int8_t val = mem.read(address);
    N = (A - val) & 0x80 != 0 ? 1 : 0;
    C = (A >= val) ? 1 : 0;
    Z = (A == val) ? 1 : 0;
}

// Compare X register
void Cpu::CPX(Memory& mem, uint16_t address)
{
    const int8_t val = mem.read(address);
    N = (X - val) & 0x80 != 0 ? 1 : 0;
    C = (X >= val) ? 1 : 0;
    Z = (X == val) ? 1 : 0;
}

// Compare Y register
void Cpu::CPY(Memory& mem, uint16_t address)
{
    const int8_t val = mem.read(address);
    N = (Y - val) & 0x80 != 0 ? 1 : 0;
    C = (Y >= val) ? 1 : 0;
    Z = (Y == val) ? 1 : 0;
}

// Decrement memory
void Cpu::DEC(Memory& mem, uint16_t address)
{
    int8_t val = mem.read(address);
    --val;
    mem.write(address, val);
    Z = (val == 0) ? 1 : 0;
    N = (val & 0x80) != 0 ? 1 : 0;
}

// Decrement X register
void Cpu::DEX()
{
    --X;
    Z = (X == 0) ? 1 : 0;
    N = (X & 0x80) != 0 ? 1 : 0;
}

// Decrement Y register
void Cpu::DEY()
{
    --Y;
    Z = (Y == 0) ? 1 : 0;
    N = (Y & 0x80) != 0 ? 1 : 0;
}

// Exclusive or
void Cpu::EOR(Memory& mem, uint16_t address)
{
    int8_t val = mem.read(address);
    A = A^val;
    Z = (A == 0) ? 1 : 0;
    N = (A & 0x80 != 0) ? 1 : 0;
}

// Increment memory
void Cpu::INC(Memory& mem, uint16_t address)
{
    int8_t val = mem.read(address);
    ++val;
    mem.write(address, val);
    Z = (val == 0) ? 1 : 0;
    N = (val & 0x80) != 0 ? 1 : 0;
}

// Increment X register
void Cpu::INX()
{
    ++X;
    Z = (X == 0) ? 1 : 0;
    N = (X & 0x80) != 0 ? 1 : 0;
}

// Increment Y register
void Cpu::INY()
{
    ++Y;
    Z = (Y == 0) ? 1 : 0;
    N = (Y & 0x80) != 0 ? 1 : 0;
}

// Jump
void Cpu::JMP(uint16_t address)
{
    // TODO

    // if addressing mode is absolute
    // PC = address;

    // if addressing mode is indirect
    // uint16_t* irqVec_p = reinterpret_cast<uint16_t*>(mem.getAddress(0xFFFE));
        // If addressing mode is indirect, don't allow locations 
        // beginning on the last byte of a page (0xXXFF)
        //PC = *irqVec_p;

    // Should probably disallow address == 0xFFFF, otherwise it'll read a byte from outside of memory

}

// Jump to subroutine
void Cpu::JSR(uint16_t address)
{
    // TODO
    
    // push PC - 1 to stack
    PC = address;

}

// Load accumulator
void Cpu::LDA(Memory& mem, uint16_t address)
{
    A = mem.read(address);
    Z = (A == 0) ? 1 : 0;
    N = (A & 0x80 != 0) ? 1 : 0;
}

// Load X register
void Cpu::LDX(Memory& mem, uint16_t address)
{
    X = mem.read(address);
    Z = (X == 0) ? 1 : 0;
    N = (X & 0x80 != 0) ? 1 : 0;
}

// Load Y register
void Cpu::LDY(Memory& mem, uint16_t address)
{
    Y = mem.read(address);
    Z = (Y == 0) ? 1 : 0;
    N = (Y & 0x80 != 0) ? 1 : 0;
}