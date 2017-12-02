#ifndef CPU_HXX
#define CPU_HXX

#include <stdint.h>
#include <string>
#include "Memory.h"
#include "Ppu.h"

namespace NesCpu {

    struct opInfo {
        opInfo() : opcode{}
            , bytes{}
            , cycles{}
        {}

        std::string opcode;
        uint8_t bytes;
        uint8_t cycles;
    };

    enum AddressMode : uint8_t
    {
        accumulator,
        absolute,
        absoluteXidx,
        absoluteYidx,
        immediate,
        implied,
        indirect,
        indirectXidx,
        indirectYidx,
        relative,
        zeropage,
        zeropageXidx,
        zeropageYidx
    };

class Cpu {
    
    
public:
    Cpu() : PC{} // Program counter
        , X{}    // X register
        , Y{}    // Y register
        , A{}    // Accumulator
        , SP{}   // Stack pointer
        , C{}    // Carry flag
        , Z{}    // Zero flag
        , I{}    // Interrupt disable
        , D{}    // Decimal mode flag
        , B{}    // Break command
        , V{}    // Overflow flag
        , N{}    // Negative flag
    {}

    uint16_t PC;
    int8_t X, Y, A;
    uint8_t SP, C, Z, I, D, B, V, N;

    static const std::string instructionTable[];

    std::string flagString();
    
    /////////////////////////////////////
    // PPU control operations
    /////////////////////////////////////

    // Enable 8x8 or 8x16 sprites
    void enableSpriteMode(Memory& mem, Ppu::SpriteType);
    
    // Disable non-maskable interrupt on V-Blank
    void disableNMI(Memory& mem);


    /////////////////////////////////////
    // Stack operations
    /////////////////////////////////////
    // Move to Memory class?

    // Push 8-bit value to stack
    void push(Memory& mem, int8_t value);

    // Push 16-bit value to stack
    void push16(Memory& mem, int16_t value);

    // Pop value from stack   
    int8_t pop(Memory& mem);

    // Pop 16-bit value from stack
    int16_t pop16(Memory& mem);

    // Peek at value on stack
    int8_t peek(Memory& mem);



    /////////////////////////////////////
    // 6502 Instructions:
    /////////////////////////////////////

    // Add with carry
    void ADC(Memory& mem, uint16_t address);

    // Logical and
    void AND(Memory& mem, uint16_t address);

    // Branch if carry clear
    void BCC(uint16_t address);

    // Branch if carry set
    void BCS(uint16_t address);

    // Branch if equal
    void BEQ(uint16_t address);

    // Bit test
    void BIT(Memory& mem, uint16_t address);

    // Branch if minus
    void BMI(uint16_t address);

    // Branch if not equal
    void BNE(uint16_t address);

    // Branch if plus
    void BPL(uint16_t address);

    // Break
    void BRK(Memory& mem);

    // Branch if overflow clear
    void BVC(uint16_t address);

    // Branch if overflow set
    void BVS(uint16_t address);

    // Clear carry flag
    void CLC();

    // Clear decimal mode
    void CLD();

    // Clear interrupt disable
    void CLI();

    // Clear overflow flag
    void CLV();

    // Compare accumulator
    void CMP(Memory& mem, uint16_t address);

    // Compare X register
    void CPX(Memory& mem, uint16_t address);

    // Compare Y register
    void CPY(Memory& mem, uint16_t address);

    // Decrement memory
    void DEC(Memory& mem, uint16_t address);

    // Decrement X register
    void DEX();

    // Decrement Y register
    void DEY();
    
    // Exclusive or
    void EOR(Memory& mem, uint16_t address);

    // Increment memory
    void INC(Memory& mem, uint16_t address);

    // Increment X register
    void INX();

    // Increment Y register
    void INY();

    // Jump
    void JMP(uint16_t address);

    // Jump to subroutine
    void JSR(Memory& mem, uint16_t address);

    // Load accumulator
    void LDA(Memory& mem, uint16_t address);
    
    // Load X register
    void LDX(Memory& mem, uint16_t address);

    // Load Y register
    void LDY(Memory& mem, uint16_t address);

    // Logical shift right
    void LSR(Memory& mem, uint16_t address);

    // No operation
    void NOP();

    // Logical inclusive or
    void ORA(Memory& mem, uint16_t address);

    // Push accumulator
    void PHA(Memory& mem);

    // Push processor status
    void PHP(Memory& mem);

    // Pull accumulator
    void PLA(Memory& mem);

    // Pull processor status
    void PLP(Memory& mem);

    // Rotate left
    void ROL(Memory& mem, uint16_t address);

    // Rotate right
    void ROR(Memory& mem, uint16_t address);

    // Return from interrupt
    void RTI(Memory& mem);

    // Return from subroutine
    void RTS(Memory& mem);

    // Subtract with carry
    void SBC(Memory& mem, uint16_t address);

    // Set carry flag
    void SEC();

    // Set decimal flag
    void SED();

    // Set interrupt flag
    void SEI();

    // Store accumulator
    void STA(Memory& mem, uint16_t address);

    // Store X register
    void STX(Memory& mem, uint16_t address);

    // Store Y register
    void STY(Memory& mem, uint16_t address);

    // Transfer accumulator to X
    void TAX();

    // Transfer accumulator to Y
    void TAY();

    // Transfer stack pointer to X
    void TSX(Memory& mem);

    // Transfer X to accumulator
    void TXA();

    // Transfer X to stack pointer
    void TXS(Memory& mem);

    // Trasnfer Y to accumulator
    void TYA();


};

}

#endif