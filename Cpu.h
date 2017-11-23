#ifndef CPU_HXX
#define CPU_HXX

#include <stdint.h>
#include <string>
#include "Memory.h"

class Cpu {
public:
    Cpu() : PC{} // Program counter
        , SP{}   // Stack pointer
        , X{}    // X register
        , Y{}    // Y register
        , A{}    // Accumulator
        , C{}    // Carry flag
        , Z{}    // Zero flag
        , I{}    // Interrupt disable
        , D{}    // Decimal mode flag
        , B{}    // Break command
        , V{}    // Overflow flag
        , N{}    // Negative flag
    {}

    uint16_t PC;
    uint8_t SP, X, Y, A;
    uint8_t C, Z, I, D, B, V, N;

    std::string flagString();
    
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
    void JSR(uint16_t address);

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
    void PHA();

    // Push processor status
    void PHP();

    // Pull accumulator
    void PLA();

    // Pull processor status
    void PLP();

    // Rotate left
    void ROL(Memory& mem, uint16_t address);

    // Rotate right
    void ROR(Memory& mem, uint16_t address);

    // Return from interrupt
    void RTI();

    // Return from subroutine
    void RTS();

    // Subtract with carry
    void SBC(Memory& mem, uint16_t address);


};

#endif