#ifndef CPU_HXX
#define CPU_HXX

#include <stdint.h>
#include <string>
#include "Memory.h"
#include "Ppu.h"

namespace NesCpu {

    struct opInfo {
        std::string opcode;
        uint8_t bytes;
        uint8_t cycles;
    };

    enum opCode : uint8_t
    {
        BRK,
        ORA_IX,
        ORA_ZP = 0x05,
        ASL_ZP,
        PHP = 0x08,
        OPA_IM,
        ASL_AC,
        OPA_AB = 0x0D,
        ASL_AB,
        BPL = 0x10,
        OPA_IY,
        ORA_ZPX = 0x15,
        ASL_ZPX,
        CLC = 0x18,
        ORA_AY,
        ORA_AX = 0x1D,
        ASL_AX,
        JSR = 0x20,
        AND_IX,
        BIT_ZP = 0x24,
        AND_ZP,
        ROL_ZP,
        PLP = 0x28,
        AND_IM,
        ROL_AC,
        BIT_AB = 0x2C,
        AND_AB,
        ROL_AB,
        BMI = 0x30,
        AND_IY,
        AND_ZPX = 0x35,
        ROL_ZPX,
        SEC = 0x38,
        AND_AY,
        AND_AX = 0x3D,
        ROL_AX,
        RTI = 0x40,
        EOR_IX,
        EOR_ZP = 0x45,
        LSR_ZP,
        PHA = 0x48,
        EOR_IM,
        LSR_AC,
        JMP_AB = 0x4C,
        EOR_AB,
        LSR_AB,
        BVC = 0x50,
        EOR_IY,
        EOR_ZPX = 0x55,
        LSR_ZPX,
        CLI = 0x58,
        EOR_AY,
        EOR_AX = 0x5D,
        LSR_AX,
        RTS = 0x60,
        ADC_IX,
        ADC_ZP = 0x65,
        ROR_ZP,
        PIA = 0x68,
        ADC_IM,
        ROR_AC,
        JMP_IN = 0x6C,
        ADC_AB,
        ROR_AB,
        BVS = 0x70,
        ADC_IY,
        ADC_ZPX = 0x65,
        ROR_ZPX,
        SEI = 0x78,
        ADC_AY,
        ADC_AX = 0x7D,
        ROR_AX,
        STA_IX = 0x81,
        STY_ZP = 0x84,
        STA_ZP,
        STX_ZP,
        DEY = 0x88,
        TXA = 0x8A,
        STY_AB = 0x8C,
        STA_AB,
        STX_AB,
        BCC = 0x90,
        STA_IY,
        STY_ZPX = 0x94,
        STA_ZPX,
        STX_ZPY,
        TYA = 0x98,
        STA_AY,
        TXS,
        STA_AX = 0x9D,
        LDY_IM = 0xA0,
        LDA_IX,
        LDX_IM,
        LDY_ZP = 0xA4,
        LDA_ZP,
        LDX_ZP,
        TAY = 0xA8,
        LDA_IM,
        TAX,
        LDY_AB = 0xAC,
        LDA_AB,
        LDX_AB,
        BCS = 0xB0,
        LDA_IY,
        LDY_ZPX = 0xB4,
        LDA_ZPX,
        LDX_ZPY,
        CLV = 0xB8,
        LDA_AY,
        TSX,
        LDY_AX = 0xBC,
        LDA_AX,
        LDX_AY,
        CPY_IM = 0xC0,
        CMP_IX,
        CPY_ZP = 0xC4,
        CMP_ZP,
        DEC_ZP,
        INY = 0xC8,
        CMP_IM,
        DEX,
        CPY_AB = 0xCC,
        CMP_AB,
        DEC_AB,
        BNE = 0xD0,
        CMP_IY,
        CMP_ZPX = 0xD5,
        DEC_ZPX,
        CLD = 0xD8,
        CMP_AY,
        CMP_AX = 0xDD,
        DEC_AX,
        CPX_IM = 0xE0,
        SBC_IX,
        CPX_ZP = 0xE4,
        SBC_ZP,
        INC_ZP,
        INX = 0xE5,
        SBC_IM,
        NOP,
        CPX_AB = 0xEC,
        SBC_AB,
        INC_AB,
        BEQ = 0xF0,
        SBC_IY,
        SBC_ZPX = 0xF5,
        INC_ZPX,
        SED = 0xF8,
        SBC_AY,
        SBC_AX = 0xFD,
        INC_AX
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

    void reset(Memory& mem);
    
    /////////////////////////////////////
    // PPU control operations
    /////////////////////////////////////

    // Enable 8x8 or 8x16 sprites
    void enableSpriteMode(Memory& mem, NesPpu::SpriteType);
    
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