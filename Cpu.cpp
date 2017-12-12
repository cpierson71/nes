#include "Cpu.h"
#include <iostream>

#define MAKE_OPINFO(idx) opinfo.bytes = opcodeByteArray[idx]; \
opinfo.operation = NesCpu::Cpu::opcodeArray[idx]


std::string NesCpu::Cpu::flagString()
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

void NesCpu::Cpu::reset(Memory& mem)
{
    PC = (mem.read(0xFFFD) << 8) + mem.read(0xFFFC);
    std::cout << (int)PC << std::endl;

    for (int i = 0; i < 15; ++i)
    {
        std::cout << (int)mem.read(PC+i) << '\n';
    }


    // TODO 

}

void NesCpu::Cpu::setupOpcodes()
{
    for (uint32_t i = 0; i < numOpcodes; ++i)
    {
        OpInfo opinfo;
        // TODO: Finish macro to update remaining info
        MAKE_OPINFO(i);
        opcodeInfoArray[i] = opinfo;
    }


}

/////////////////////////////////////
// PPU control operations
/////////////////////////////////////

// Enable 8x8 or 8x16 sprites
void NesCpu::Cpu::enableSpriteMode(Memory& mem, NesPpu::SpriteType spriteType)
{
    // Setting bit 5 of $2000 enables 8x16 sprites
    const uint8_t bitNum = 5;
    // 8x8 == 0, 8x16 == 1
    mem.setBit(NesPpu::ppuControlRegister1, bitNum, spriteType);
}

// Disable non-maskable interrupt (NMI) on V-Blank
void NesCpu::Cpu::disableNMI(Memory& mem)
{
    // Setting bit 7 of $2000 disables NMI
    const uint8_t bitNum = 7;
    mem.setBit(NesPpu::ppuControlRegister1, bitNum, 0);

}

/////////////////////////////////////
// Stack operations
/////////////////////////////////////

// Push 8-bit value to stack
void NesCpu::Cpu::push(Memory& mem, int8_t value)
{
    // Stack is located in memory locations $0100-$01FF and works top-down
    uint16_t address = 0x1FF - SP;
    mem.write(address, value);
    --SP; // Intentionally not preventing overflow
}

// Push 16-bit value to stack
void NesCpu::Cpu::push16(Memory& mem, int16_t value)
{
    // Stack is located in memory locations $0100-$01FF and works top-down
    uint16_t address = 0x1FF - SP;
    uint8_t* bytes = reinterpret_cast<uint8_t*>(mem.getAddress(address));
    // TODO -- verify order
    // Push each byte of the uint16_t value
    mem.write(address, bytes[0]);
    --SP; // Intentionally not preventing overflow
    mem.write(address, bytes[1]);
    --SP; // Intentionally not preventing overflow
}

// Pop value from stack 
int8_t NesCpu::Cpu::pop(Memory& mem)
{
    // Stack is located in memory locations $0100-$01FF and works top-down
    uint16_t address = 0x1FF - SP;
    int8_t val = mem.read(address);
    ++SP; // Intentionally not preventing overflow
    return val;
}

// Pop 16-bit value from stack
int16_t NesCpu::Cpu::pop16(Memory& mem)
{
    // Stack is located in memory locations $0100-$01FF and works top-down
    uint16_t address = 0x1FF - SP;
    int8_t bytes[2];
    // TODO -- verify order
    // Pop each byte of the uint16_t value
    bytes[0] = mem.read(address);
    ++SP; // Intentionally not preventing overflow
    bytes[1] = mem.read(address);
    ++SP; // Intentionally not preventing overflow
    uint16_t* value_p = reinterpret_cast<uint16_t*>(bytes);
    return *value_p;
}

// Peek at value on stack
int8_t NesCpu::Cpu::peek(Memory& mem)
{
    // Stack is located in memory locations $0100-$01FF and works top-down
    uint16_t address = 0x1FF - SP;
    int8_t val = mem.read(address);
    // Don't increment SP
    return val;
}

/////////////////////////////////////
// 6502 Instructions:
/////////////////////////////////////

// Add with carry
void NesCpu::Cpu::ADC(Memory& mem, uint16_t address)
{
    const int8_t val = mem.read(address);
    const uint8_t prevA = A;
    A = A + val + C;
    Z = (A == 0) ? 1 : 0;
    C = (static_cast<int>(prevA) + val + C > 0xFF) ? 1 : 0;
    N = (A & 0x80 != 0) ? 1 : 0;
    // Overflow occurred if accumulator flipped signs after addition && 
    // accumulator and memory value are same sign
    V = ((prevA^A) & 0x80 != 0 && (A^val) & 0x80 == 0) ? 1 : 0;
}

// Logical and
void NesCpu::Cpu::AND(Memory& mem, uint16_t address)
{
    const int8_t val = mem.read(address);
    A = A & val;
    Z = (A == 0) ? 1 : 0;
    N = (A & 0x80 != 0) ? 1 : 0;
}

// Arithmetic shift left
void NesCpu::Cpu::ASL(Memory& mem, uint16_t address)
{
    // TODO: Check for accumulator address mode
    int8_t val = mem.read(address);
    C = (val & 0x80 != 0) ? 1 : 0;
    val <<= 2;
    Z = (A == 0) ? 1 : 0;
    N = (val & 0x80 != 0) ? 1 : 0;
    mem.write(address, val);
}

// Branch on carry clear
void NesCpu::Cpu::BCC(Memory& mem, uint16_t address)
{
    if(C == 0)
    {
        PC = address;
    }
}

// Branch on carry set
void NesCpu::Cpu::BCS(Memory& mem, uint16_t address)
{
    if(C == 1)
    {
        PC = address;
    }
}

// Branch on equal
void NesCpu::Cpu::BEQ(Memory& mem, uint16_t address)
{
    if(Z == 1)
    {
        PC = address;
    }
}

// Bit test
void NesCpu::Cpu::BIT(Memory& mem, uint16_t address)
{
    const int8_t val = mem.read(address);
    Z = (A & val) == 0 ? 1 : 0;
    N = val & 0x80;
    V = val & 0x40;
}

// Branch if minus
void NesCpu::Cpu::BMI(Memory& mem, uint16_t address)
{
    if (N == 1)
    {
        PC = address;
    }
}

// Branch if not equal
void NesCpu::Cpu::BNE(Memory& mem, uint16_t address)
{
    if (Z == 0)
    {
        PC = address;
    }
}

// Branch if plus
void NesCpu::Cpu::BPL(Memory& mem, uint16_t address)
{
    if (N == 0)
    {
        PC = address;
    }
}

// Break
void NesCpu::Cpu::BRK(Memory& mem, uint16_t address)
{
    push16(mem, PC);
    PHP(mem,0);
    uint16_t* irqVec_p = reinterpret_cast<uint16_t*>(mem.getAddress(0xFFFE));
    PC = *irqVec_p;
    B = 1;

    std::cout << "BRK\n";

}

// Branch if overflow clear
void NesCpu::Cpu::BVC(Memory& mem, uint16_t address)
{
    if (V == 0)
    {
        PC = address;
    }
}

// Branch if overflow set
void NesCpu::Cpu::BVS(Memory& mem, uint16_t address)
{
    if (V == 1)
    {
        PC = address;
    }
}

// Clear carry flag
void NesCpu::Cpu::CLC(Memory& mem, uint16_t address)
{
    C = 0;
}

// Clear decimal mode
void NesCpu::Cpu::CLD(Memory& mem, uint16_t address)
{
    D = 0;
}

// Clear interrupt disable
void NesCpu::Cpu::CLI(Memory& mem, uint16_t address)
{
    I = 0;
}

// Clear overflow flag
void NesCpu::Cpu::CLV(Memory& mem, uint16_t address)
{
    V = 0;
}

// Compare accumulator
void NesCpu::Cpu::CMP(Memory& mem, uint16_t address)
{
    const int8_t val = mem.read(address);
    N = (A - val) & 0x80 != 0 ? 1 : 0;
    C = (A >= val) ? 1 : 0;
    Z = (A == val) ? 1 : 0;
}

// Compare X register
void NesCpu::Cpu::CPX(Memory& mem, uint16_t address)
{
    const int8_t val = mem.read(address);
    N = (X - val) & 0x80 != 0 ? 1 : 0;
    C = (X >= val) ? 1 : 0;
    Z = (X == val) ? 1 : 0;
}

// Compare Y register
void NesCpu::Cpu::CPY(Memory& mem, uint16_t address)
{
    const int8_t val = mem.read(address);
    N = (Y - val) & 0x80 != 0 ? 1 : 0;
    C = (Y >= val) ? 1 : 0;
    Z = (Y == val) ? 1 : 0;
}

// Decrement memory
void NesCpu::Cpu::DEC(Memory& mem, uint16_t address)
{
    int8_t val = mem.read(address);
    --val;
    mem.write(address, val);
    Z = (val == 0) ? 1 : 0;
    N = (val & 0x80) != 0 ? 1 : 0;
}

// Decrement X register
void NesCpu::Cpu::DEX(Memory& mem, uint16_t address)
{
    --X;
    Z = (X == 0) ? 1 : 0;
    N = (X & 0x80) != 0 ? 1 : 0;
}

// Decrement Y register
void NesCpu::Cpu::DEY(Memory& mem, uint16_t address)
{
    --Y;
    Z = (Y == 0) ? 1 : 0;
    N = (Y & 0x80) != 0 ? 1 : 0;
}

// Logical exclusive or
void NesCpu::Cpu::EOR(Memory& mem, uint16_t address)
{
    int8_t val = mem.read(address);
    A = A^val;
    Z = (A == 0) ? 1 : 0;
    N = (A & 0x80 != 0) ? 1 : 0;
}

// Increment memory
void NesCpu::Cpu::INC(Memory& mem, uint16_t address)
{
    int8_t val = mem.read(address);
    ++val;
    mem.write(address, val);
    Z = (val == 0) ? 1 : 0;
    N = (val & 0x80) != 0 ? 1 : 0;
}

// Increment X register
void NesCpu::Cpu::INX(Memory& mem, uint16_t address)
{
    ++X;
    Z = (X == 0) ? 1 : 0;
    N = (X & 0x80) != 0 ? 1 : 0;
}

// Increment Y register
void NesCpu::Cpu::INY(Memory& mem, uint16_t address)
{
    ++Y;
    Z = (Y == 0) ? 1 : 0;
    N = (Y & 0x80) != 0 ? 1 : 0;
}

// Jump
void NesCpu::Cpu::JMP(Memory& mem, uint16_t address)
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
void NesCpu::Cpu::JSR(Memory& mem, uint16_t address)
{
    push16(mem, PC - 1);
    PC = address;
}

// Load accumulator
void NesCpu::Cpu::LDA(Memory& mem, uint16_t address)
{
    A = mem.read(address);
    Z = (A == 0) ? 1 : 0;
    N = (A & 0x80 != 0) ? 1 : 0;
}

// Load X register
void NesCpu::Cpu::LDX(Memory& mem, uint16_t address)
{
    X = mem.read(address);
    Z = (X == 0) ? 1 : 0;
    N = (X & 0x80 != 0) ? 1 : 0;
}

// Load Y register
void NesCpu::Cpu::LDY(Memory& mem, uint16_t address)
{
    Y = mem.read(address);
    Z = (Y == 0) ? 1 : 0;
    N = (Y & 0x80 != 0) ? 1 : 0;
}

// Logical shift right
void NesCpu::Cpu::LSR(Memory& mem, uint16_t address)
{
    // TODO
    // If in accumulator mode, perform on A instead

    // Cast as uint8_t so when the bits are shifted, the top bit becomes 0
    // For int8_t, the top bit will stay the same
    uint8_t val = static_cast<uint8_t>(mem.read(address));
    C = val & 0x01;
    val >>= 1;
    mem.write(address, static_cast<int8_t>(val));
    Z = (val == 0) ? 1 : 0;
    N = (val & 0x80 != 0) ? 1 : 0;
}

// No operation
void NesCpu::Cpu::NOP(Memory& mem, uint16_t address)
{
    // Intentionally empty
}

// Logical inclusive or
void NesCpu::Cpu::ORA(Memory& mem, uint16_t address)
{
    const int8_t val = mem.read(address);
    A = A|val;
    Z = (A == 0) ? 1 : 0;
    N = (A & 0x80 != 0) ? 1 : 0;
}

// Push accumulator
void NesCpu::Cpu::PHA(Memory& mem, uint16_t address)
{
    push(mem, A);
}

// Push processor status
void NesCpu::Cpu::PHP(Memory& mem, uint16_t address)
{
    // Reformat status flags into a single byte arranged as follows:
    // Bits:  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
    // Flags: | N | V |   | B | D | I | Z | C |
    uint8_t byteFlag = 0;
    if (C) {byteFlag += 1;}
    if (Z) {byteFlag += 2;}
    if (I) {byteFlag += 4;}
    if (D) {byteFlag += 8;}
    if (B) {byteFlag += 16;}
    if (V) {byteFlag += 64;}
    if (N) {byteFlag += 128;}

    int8_t* byteFlag_p = reinterpret_cast<int8_t*>(&byteFlag);
    push(mem, *byteFlag_p);
}

// Pull accumulator
void NesCpu::Cpu::PLA(Memory& mem, uint16_t address)
{
    A = pop(mem);
    Z = (A == 0) ? 1 : 0;
    N = (A & 0x80 != 0) ? 1 : 0;
}

// Pull processor status
void NesCpu::Cpu::PLP(Memory& mem, uint16_t address)
{
    // Reformat status flags from a single byte arranged as follows:
    // Bits:  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
    // Flags: | N | V |   | B | D | I | Z | C |
    int8_t byteFlags = pop(mem);

    // Set status flags to values pulled
    N = byteFlags & 0x80;
    V = byteFlags & 0x40;
    B = byteFlags & 0x10;
    D = byteFlags & 0x08;
    I = byteFlags & 0x04;
    Z = byteFlags & 0x02;
    C = byteFlags & 0x01;
}

// Rotate left
void NesCpu::Cpu::ROL(Memory& mem, uint16_t address)
{
    // TODO
    // If in accumulator mode, perform on A instead

    int8_t val = mem.read(address);
    const int8_t signBit = val & 0x80;
    val <<= 1;
    if (C == 1)
    {
        val |= 0x01;
    }
    C = signBit;
    Z = (val == 0) ? 1 : 0;
    N = (val & 0x80 != 0) ? 1 : 0;
}

// Rotate right
void NesCpu::Cpu::ROR(Memory& mem, uint16_t address)
{
    // TODO
    // If in accumulator mode, perform on A instead

    int8_t val = mem.read(address);
    const int8_t zeroBit = val & 0x01;
    val >>= 1;
    if (C == 1)
    {
        val |= 0x80;
    }
    C = zeroBit;
    Z = (val == 0) ? 1 : 0;
    N = (val & 0x80 != 0) ? 1 : 0;
}

// Return from interrupt
void NesCpu::Cpu::RTI(Memory& mem, uint16_t address)
{
    PLP(mem,0);
    PC = pop16(mem);
}

// Return from subroutine
void NesCpu::Cpu::RTS(Memory& mem, uint16_t address)
{
    PC = pop(mem); 
}

// Subtract with carry
void NesCpu::Cpu::SBC(Memory& mem, uint16_t address)
{
    const int8_t val = mem.read(address);
    const int8_t prevA = A;
    A = A - val - (!C);
    C = (static_cast<int>(prevA) - val - (!C) < -128) ? 1 : 0;
    Z = (A == 0) ? 1 : 0;
    N = (A & 0x80 != 0) ? 1 : 0;
    // Overflow occurred if accumulator flipped signs after subtraction && 
    // accumulator and memory value are same sign
    V = ((prevA^A) & 0x80 != 0 && (A^val) & 0x80 == 0) ? 1 : 0;
}

// Set carry flag
void NesCpu::Cpu::SEC(Memory& mem, uint16_t address)
{
    C = 1;
}

// Set decimal flag
void NesCpu::Cpu::SED(Memory& mem, uint16_t address)
{
    D = 1;
}

// Set interrupt flag
void NesCpu::Cpu::SEI(Memory& mem, uint16_t address)
{
    I = 1;
}

// Store accumulator
void NesCpu::Cpu::STA(Memory& mem, uint16_t address)
{
    mem.write(address, A);
}

// Store X register
void NesCpu::Cpu::STX(Memory& mem, uint16_t address)
{
    mem.write(address, X);
}

// Store Y register
void NesCpu::Cpu::STY(Memory& mem, uint16_t address)
{
    mem.write(address, Y);
}

// Transfer accumulator to X
void NesCpu::Cpu::TAX(Memory& mem, uint16_t address)
{
    X = A;
    Z = (X == 0) ? 1 : 0;
    N = (X & 0x80 != 0) ? 1 : 0;
}

// Transfer accumulator to Y
void NesCpu::Cpu::TAY(Memory& mem, uint16_t address)
{
    Y = A;
    Z = (Y == 0) ? 1 : 0;
    N = (Y & 0x80 != 0) ? 1 : 0;
}

// Transfer stack pointer to X
void NesCpu::Cpu::TSX(Memory& mem, uint16_t address)
{
    X = peek(mem);
    Z = (X == 0) ? 1 : 0;
    N = (X & 0x80 != 0) ? 1 : 0;
}

// Transfer X to accumulator
void NesCpu::Cpu::TXA(Memory& mem, uint16_t address)
{
    A = X;
    Z = (A == 0) ? 1 : 0;
    N = (A & 0x80 != 0) ? 1 : 0;
}

// Transfer X to stack pointer
void NesCpu::Cpu::TXS(Memory& mem, uint16_t address)
{
    push(mem, X);
}

// Transfer Y to accumulator
void NesCpu::Cpu::TYA(Memory& mem, uint16_t address)
{
    A = Y;
    Z = (A == 0) ? 1 : 0;
    N = (A & 0x80 != 0) ? 1 : 0;
}

// For unused opcodes
void NesCpu::Cpu::UNK(Memory& mem, uint16_t address)
{
    std::cout << "Error: Unused opcode used\n";
}

/*
const std::string NesCpu::Cpu::opcodeNameArray[] = {
    "BRK","ORA","---","---","---","ORA","ASL","---","PHP","ORA","ASL","---","---","ORA","ASL","---",
    "BPL","ORA","---","---","---","ORA","ASL","---","CLC","ORA","---","---","---","ORA","ASL","---",
    "JSR","AND","---","---","BIT","AND","ROL","---","PLP","AND","ROL","---","BIT","AND","ROL","---",
    "BMI","AND","---","---","---","AND","ROL","---","SEC","AND","---","---","---","AND","ROL","---",
    "RTI","EOR","---","---","---","EOR","LSR","---","PHA","EOR","LSR","---","JMP","EOR","LSR","---",
    "BVC","EOR","---","---","---","EOR","LSR","---","CLI","EOR","---","---","---","EOR","LSR","---",
    "RTS","ADC","---","---","---","ADC","ROR","---","PLA","ADC","ROR","---","JMP","ADC","ROR","---",
    "BVS","ADC","---","---","---","ADC","ROR","---","SEI","ADC","---","---","---","ADC","ROR","---",
    "---","STA","---","---","STY","STA","STX","---","DEY","---","TXA","---","STY","STA","STX","---",
    "BCC","STA","---","---","STY","STA","STX","---","TYA","STA","TXS","---","---","STA","---","---",
    "LDY","LDA","LDX","---","LDY","LDA","LDX","---","TAY","LDA","TAX","---","LDY","LDA","LDX","---",
    "BCS","LDA","---","---","LDY","LDA","LDX","---","CLV","LDA","TSX","---","LDY","LDA","LDX","---",
    "CPY","CMP","---","---","CPY","CMP","DEC","---","INY","CMP","DEX","---","CPY","CMP","DEC","---",
    "BNE","CMP","---","---","---","CMP","DEC","---","CLD","CMP","---","---","---","CMP","DEC","---",
    "CPX","SBC","---","---","CPX","SBC","INC","---","INX","SBC","NOP","---","CPX","SBC","INC","---",
    "BEQ","SBC","---","---","---","SBC","INC","---","SED","SBC","---","---","---","SBC","INC","---"};
*/

const uint8_t NesCpu::Cpu::opcodeByteArray[] = {
    1, 2, 0, 0, 0, 2, 2, 0, 1, 2, 1, 0, 0, 3, 3, 0,
    2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0,
    3, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0,
    1, 2, 0, 0, 0, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0,
    1, 2, 0, 0, 0, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0,
    0, 2, 0, 0, 2, 2, 2, 0, 1, 0, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 0, 3, 0, 0,
    2, 2, 2, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 1, 3, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0,
    2, 2, 0, 0, 2, 2, 2, 0, 1, 2, 1, 0, 3, 3, 3, 0,
    2, 2, 0, 0, 0, 2, 2, 0, 1, 3, 0, 0, 0, 3, 3, 0};

const std::function<void(NesCpu::Cpu&, Memory&, uint16_t)> NesCpu::Cpu::opcodeArray[] = {
    NesCpu::Cpu::BRK, NesCpu::Cpu::ORA, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::ORA, NesCpu::Cpu::ASL, NesCpu::Cpu::UNK, NesCpu::Cpu::PHP, NesCpu::Cpu::ORA, NesCpu::Cpu::ASL, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::ORA, NesCpu::Cpu::ASL, NesCpu::Cpu::UNK, NesCpu::Cpu::
    BPL, NesCpu::Cpu::ORA, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::ORA, NesCpu::Cpu::ASL, NesCpu::Cpu::UNK, NesCpu::Cpu::CLC, NesCpu::Cpu::ORA, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::ORA, NesCpu::Cpu::ASL, NesCpu::Cpu::UNK, NesCpu::Cpu::
    JSR, NesCpu::Cpu::AND, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::BIT, NesCpu::Cpu::AND, NesCpu::Cpu::ROL, NesCpu::Cpu::UNK, NesCpu::Cpu::PLP, NesCpu::Cpu::AND, NesCpu::Cpu::ROL, NesCpu::Cpu::UNK, NesCpu::Cpu::BIT, NesCpu::Cpu::AND, NesCpu::Cpu::ROL, NesCpu::Cpu::UNK, NesCpu::Cpu::
    BMI, NesCpu::Cpu::AND, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::AND, NesCpu::Cpu::ROL, NesCpu::Cpu::UNK, NesCpu::Cpu::SEC, NesCpu::Cpu::AND, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::AND, NesCpu::Cpu::ROL, NesCpu::Cpu::UNK, NesCpu::Cpu::
    RTI, NesCpu::Cpu::EOR, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::EOR, NesCpu::Cpu::LSR, NesCpu::Cpu::UNK, NesCpu::Cpu::PHA, NesCpu::Cpu::EOR, NesCpu::Cpu::LSR, NesCpu::Cpu::UNK, NesCpu::Cpu::JMP, NesCpu::Cpu::EOR, NesCpu::Cpu::LSR, NesCpu::Cpu::UNK, NesCpu::Cpu::
    BVC, NesCpu::Cpu::EOR, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::EOR, NesCpu::Cpu::LSR, NesCpu::Cpu::UNK, NesCpu::Cpu::CLI, NesCpu::Cpu::EOR, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::EOR, NesCpu::Cpu::LSR, NesCpu::Cpu::UNK, NesCpu::Cpu::
    RTS, NesCpu::Cpu::ADC, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::ADC, NesCpu::Cpu::ROR, NesCpu::Cpu::UNK, NesCpu::Cpu::PLA, NesCpu::Cpu::ADC, NesCpu::Cpu::ROR, NesCpu::Cpu::UNK, NesCpu::Cpu::JMP, NesCpu::Cpu::ADC, NesCpu::Cpu::ROR, NesCpu::Cpu::UNK, NesCpu::Cpu::
    BVS, NesCpu::Cpu::ADC, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::ADC, NesCpu::Cpu::ROR, NesCpu::Cpu::UNK, NesCpu::Cpu::SEI, NesCpu::Cpu::ADC, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::ADC, NesCpu::Cpu::ROR, NesCpu::Cpu::UNK, NesCpu::Cpu::
    UNK, NesCpu::Cpu::STA, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::STY, NesCpu::Cpu::STA, NesCpu::Cpu::STX, NesCpu::Cpu::UNK, NesCpu::Cpu::DEY, NesCpu::Cpu::UNK, NesCpu::Cpu::TXA, NesCpu::Cpu::UNK, NesCpu::Cpu::STY, NesCpu::Cpu::STA, NesCpu::Cpu::STX, NesCpu::Cpu::UNK, NesCpu::Cpu::
    BCC, NesCpu::Cpu::STA, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::STY, NesCpu::Cpu::STA, NesCpu::Cpu::STX, NesCpu::Cpu::UNK, NesCpu::Cpu::TYA, NesCpu::Cpu::STA, NesCpu::Cpu::TXS, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::STA, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::
    LDY, NesCpu::Cpu::LDA, NesCpu::Cpu::LDX, NesCpu::Cpu::UNK, NesCpu::Cpu::LDY, NesCpu::Cpu::LDA, NesCpu::Cpu::LDX, NesCpu::Cpu::UNK, NesCpu::Cpu::TAY, NesCpu::Cpu::LDA, NesCpu::Cpu::TAX, NesCpu::Cpu::UNK, NesCpu::Cpu::LDY, NesCpu::Cpu::LDA, NesCpu::Cpu::LDX, NesCpu::Cpu::UNK, NesCpu::Cpu::
    BCS, NesCpu::Cpu::LDA, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::LDY, NesCpu::Cpu::LDA, NesCpu::Cpu::LDX, NesCpu::Cpu::UNK, NesCpu::Cpu::CLV, NesCpu::Cpu::LDA, NesCpu::Cpu::TSX, NesCpu::Cpu::UNK, NesCpu::Cpu::LDY, NesCpu::Cpu::LDA, NesCpu::Cpu::LDX, NesCpu::Cpu::UNK, NesCpu::Cpu::
    CPY, NesCpu::Cpu::CMP, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::CPY, NesCpu::Cpu::CMP, NesCpu::Cpu::DEC, NesCpu::Cpu::UNK, NesCpu::Cpu::INY, NesCpu::Cpu::CMP, NesCpu::Cpu::DEX, NesCpu::Cpu::UNK, NesCpu::Cpu::CPY, NesCpu::Cpu::CMP, NesCpu::Cpu::DEC, NesCpu::Cpu::UNK, NesCpu::Cpu::
    BNE, NesCpu::Cpu::CMP, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::CMP, NesCpu::Cpu::DEC, NesCpu::Cpu::UNK, NesCpu::Cpu::CLD, NesCpu::Cpu::CMP, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::CMP, NesCpu::Cpu::DEC, NesCpu::Cpu::UNK, NesCpu::Cpu::
    CPX, NesCpu::Cpu::SBC, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::CPX, NesCpu::Cpu::SBC, NesCpu::Cpu::INC, NesCpu::Cpu::UNK, NesCpu::Cpu::INX, NesCpu::Cpu::SBC, NesCpu::Cpu::NOP, NesCpu::Cpu::UNK, NesCpu::Cpu::CPX, NesCpu::Cpu::SBC, NesCpu::Cpu::INC, NesCpu::Cpu::UNK, NesCpu::Cpu::
    BEQ, NesCpu::Cpu::SBC, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::SBC, NesCpu::Cpu::INC, NesCpu::Cpu::UNK, NesCpu::Cpu::SED, NesCpu::Cpu::SBC, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::UNK, NesCpu::Cpu::SBC, NesCpu::Cpu::INC, NesCpu::Cpu::UNK};

