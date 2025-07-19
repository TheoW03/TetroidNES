#include <unordered_map>
#include <stdint.h>

#include <Emulator/InstructionMap.h>

static const std::unordered_map<uint8_t, Instruction> INSTRUCTION_MAP = {
// LDA
    {0xA9, Instruction{InstFunc::LDA_PTR, AddressMode::IMMEDIATE}},
    {0xA5, Instruction{InstFunc::LDA_PTR, AddressMode::ZERO_PAGE}},
    {0xA5, Instruction{InstFunc::LDA_PTR, AddressMode::ZERO_PAGE_X}},
    {0xAD, Instruction{InstFunc::LDA_PTR, AddressMode::ABSOLUTE}},
    {0xA9, Instruction{InstFunc::LDA_PTR, AddressMode::IMMEDIATE}},
    {0xBD, Instruction{InstFunc::LDA_PTR, AddressMode::ABSOLUTE_X}},
    {0xB9, Instruction{InstFunc::LDA_PTR, AddressMode::ABSOLUTE_Y}},
    {0xA1, Instruction{InstFunc::LDA_PTR, AddressMode::INDIRECT_X}},
    {0xB1, Instruction{InstFunc::LDA_PTR, AddressMode::INDIRECT_Y}},

// LDX
    {0xA2, Instruction{InstFunc::LDX_PTR, AddressMode::IMMEDIATE}},
    {0xA6, Instruction{InstFunc::LDX_PTR, AddressMode::ZERO_PAGE}},
    {0xB6, Instruction{InstFunc::LDX_PTR, AddressMode::ZERO_PAGE_Y}},
    {0xAE, Instruction{InstFunc::LDX_PTR, AddressMode::ABSOLUTE}},
    {0xBE, Instruction{InstFunc::LDX_PTR, AddressMode::ABSOLUTE_Y}},


// LDY
    {0xA0, Instruction{InstFunc::LDY_PTR, AddressMode::IMMEDIATE}},
    {0xA4, Instruction{InstFunc::LDY_PTR, AddressMode::ZERO_PAGE}},
    {0xB4, Instruction{InstFunc::LDY_PTR, AddressMode::ZERO_PAGE_X}},
    {0xAC, Instruction{InstFunc::LDY_PTR, AddressMode::ABSOLUTE}},
    {0xBC, Instruction{InstFunc::LDY_PTR, AddressMode::ABSOLUTE_X}},


// ADC
    {0x69, Instruction{InstFunc::ADC_PTR, AddressMode::IMMEDIATE}},
    {0x65, Instruction{InstFunc::ADC_PTR, AddressMode::ZERO_PAGE}},
    {0x75, Instruction{InstFunc::ADC_PTR, AddressMode::ZERO_PAGE_X}},
    {0x6D, Instruction{InstFunc::ADC_PTR, AddressMode::ABSOLUTE}},
    {0x7D, Instruction{InstFunc::ADC_PTR, AddressMode::ABSOLUTE_X}},
    {0x79, Instruction{InstFunc::ADC_PTR, AddressMode::ABSOLUTE_Y}},
    {0x61, Instruction{InstFunc::ADC_PTR, AddressMode::INDIRECT_X}},
    {0x71, Instruction{InstFunc::ADC_PTR, AddressMode::INDIRECT_Y}},


// SBC
    {0xE9, Instruction{InstFunc::SBC_PTR, AddressMode::IMMEDIATE}},
    {0xE5, Instruction{InstFunc::SBC_PTR, AddressMode::ZERO_PAGE}},
    {0xF5, Instruction{InstFunc::SBC_PTR, AddressMode::ZERO_PAGE_X}},
    {0xED, Instruction{InstFunc::SBC_PTR, AddressMode::ABSOLUTE}},
    {0xFD, Instruction{InstFunc::SBC_PTR, AddressMode::ABSOLUTE_X}},
    {0xF9, Instruction{InstFunc::SBC_PTR, AddressMode::ABSOLUTE_Y}},
    {0xE1, Instruction{InstFunc::SBC_PTR, AddressMode::INDIRECT_X}},
    {0xF1, Instruction{InstFunc::SBC_PTR, AddressMode::INDIRECT_Y}},


// AND
    {0x29, Instruction{InstFunc::AND_PTR, AddressMode::IMMEDIATE}},
    {0x25, Instruction{InstFunc::AND_PTR, AddressMode::ZERO_PAGE}},
    {0x35, Instruction{InstFunc::AND_PTR, AddressMode::ZERO_PAGE_X}},
    {0x2D, Instruction{InstFunc::AND_PTR, AddressMode::ABSOLUTE}},
    {0x3D, Instruction{InstFunc::AND_PTR, AddressMode::ABSOLUTE_X}},
    {0x39, Instruction{InstFunc::AND_PTR, AddressMode::ABSOLUTE_Y}},
    {0x21, Instruction{InstFunc::AND_PTR, AddressMode::INDIRECT_X}},
    {0x31, Instruction{InstFunc::AND_PTR, AddressMode::INDIRECT_Y}},


// ORA
    {0x09, Instruction{InstFunc::ORA_PTR, AddressMode::IMMEDIATE}},
    {0x05, Instruction{InstFunc::ORA_PTR, AddressMode::ZERO_PAGE}},
    {0x15, Instruction{InstFunc::ORA_PTR, AddressMode::ZERO_PAGE_X}},
    {0x0D, Instruction{InstFunc::ORA_PTR, AddressMode::ABSOLUTE}},
    {0x1D, Instruction{InstFunc::ORA_PTR, AddressMode::ABSOLUTE_X}},
    {0x19, Instruction{InstFunc::ORA_PTR, AddressMode::ABSOLUTE_Y}},
    {0x01, Instruction{InstFunc::ORA_PTR, AddressMode::INDIRECT_X}},
    {0x11, Instruction{InstFunc::ORA_PTR, AddressMode::INDIRECT_Y}},


// ROL/ROR
    {0x2A, Instruction{InstFunc::ROL_PTR, AddressMode::ACCUMULATOR}},
    {0x26, Instruction{InstFunc::ROL_PTR, AddressMode::ZERO_PAGE}},
    {0x36, Instruction{InstFunc::ROL_PTR, AddressMode::ZERO_PAGE_X}},
    {0x2E, Instruction{InstFunc::ROL_PTR, AddressMode::ABSOLUTE}},
    {0x3E, Instruction{InstFunc::ROL_PTR, AddressMode::ABSOLUTE_X}},

    {0x6A, Instruction{InstFunc::ROR_PTR, AddressMode::ACCUMULATOR}},
    {0x66, Instruction{InstFunc::ROR_PTR, AddressMode::ZERO_PAGE}},
    {0x76, Instruction{InstFunc::ROR_PTR, AddressMode::ZERO_PAGE_X}},
    {0x6E, Instruction{InstFunc::ROR_PTR, AddressMode::ABSOLUTE}},
    {0x7E, Instruction{InstFunc::ROR_PTR, AddressMode::ABSOLUTE_X}},


// STX/STY/STA
    {0x86, Instruction{InstFunc::STX_PTR, AddressMode::ZERO_PAGE}},
    {0x96, Instruction{InstFunc::STX_PTR, AddressMode::ZERO_PAGE_X}},
    {0x8E, Instruction{InstFunc::STX_PTR, AddressMode::ABSOLUTE}},

    {0x84, Instruction{InstFunc::STY_PTR, AddressMode::ZERO_PAGE}},
    {0x94, Instruction{InstFunc::STY_PTR, AddressMode::ZERO_PAGE_X}},
    {0x8C, Instruction{InstFunc::STY_PTR, AddressMode::ABSOLUTE}},

    {0x85, Instruction{InstFunc::STA_PTR, AddressMode::ZERO_PAGE}},
    {0x95, Instruction{InstFunc::STA_PTR, AddressMode::ZERO_PAGE_X}},
    {0x8D, Instruction{InstFunc::STA_PTR, AddressMode::ABSOLUTE}},
    {0x9D, Instruction{InstFunc::STA_PTR, AddressMode::ABSOLUTE_X}},
    {0x99, Instruction{InstFunc::STA_PTR, AddressMode::ABSOLUTE_Y}},
    {0x81, Instruction{InstFunc::STA_PTR, AddressMode::INDIRECT_X}},
    {0x91, Instruction{InstFunc::STA_PTR, AddressMode::INDIRECT_Y}},


// bitshift
    {0x4A, Instruction{InstFunc::LSR_PTR, AddressMode::ACCUMULATOR}},
    {0x46, Instruction{InstFunc::LSR_PTR, AddressMode::ZERO_PAGE}},
    {0x56, Instruction{InstFunc::LSR_PTR, AddressMode::ZERO_PAGE_X}},
    {0x4E, Instruction{InstFunc::LSR_PTR, AddressMode::ABSOLUTE}},
    {0x5E, Instruction{InstFunc::LSR_PTR, AddressMode::ABSOLUTE_X}},

    {0x0A, Instruction{InstFunc::ASL_PTR, AddressMode::ACCUMULATOR}},
    {0x06, Instruction{InstFunc::ASL_PTR, AddressMode::ZERO_PAGE}},
    {0x16, Instruction{InstFunc::ASL_PTR, AddressMode::ZERO_PAGE_X}},
    {0x0E, Instruction{InstFunc::ASL_PTR, AddressMode::ABSOLUTE}},
    {0x1E, Instruction{InstFunc::ASL_PTR, AddressMode::ABSOLUTE_X}},


// XOR
    {0x49, Instruction{InstFunc::EOR_PTR, AddressMode::IMMEDIATE}},
    {0x45, Instruction{InstFunc::EOR_PTR, AddressMode::ZERO_PAGE}},
    {0x55, Instruction{InstFunc::EOR_PTR, AddressMode::ZERO_PAGE_X}},
    {0x4D, Instruction{InstFunc::EOR_PTR, AddressMode::ABSOLUTE}},
    {0x5D, Instruction{InstFunc::EOR_PTR, AddressMode::ABSOLUTE_X}},
    {0x59, Instruction{InstFunc::EOR_PTR, AddressMode::ABSOLUTE_Y}},
    {0x41, Instruction{InstFunc::EOR_PTR, AddressMode::INDIRECT_X}},
    {0x51, Instruction{InstFunc::EOR_PTR, AddressMode::INDIRECT_Y}},


// DEC
    {0xCA, Instruction{InstFunc::DEX_PTR, AddressMode::IMPLIED}},
    {0x88, Instruction{InstFunc::DEY_PTR, AddressMode::IMPLIED}},

    {0xC6, Instruction{InstFunc::DEC_PTR, AddressMode::ZERO_PAGE}},
    {0xD6, Instruction{InstFunc::DEC_PTR, AddressMode::ZERO_PAGE_X}},
    {0xCE, Instruction{InstFunc::DEC_PTR, AddressMode::ABSOLUTE}},
    {0xDE, Instruction{InstFunc::DEC_PTR, AddressMode::ABSOLUTE_X}},


// INC
    {0xE8, Instruction{InstFunc::INX_PTR, AddressMode::IMPLIED}},

    {0xC8, Instruction{InstFunc::INY_PTR, AddressMode::IMPLIED}},

    {0xE6, Instruction{InstFunc::INC_PTR, AddressMode::ZERO_PAGE}},
    {0xF6, Instruction{InstFunc::INC_PTR, AddressMode::ZERO_PAGE_X}},
    {0xEE, Instruction{InstFunc::INC_PTR, AddressMode::ABSOLUTE}},
    {0xFE, Instruction{InstFunc::INC_PTR, AddressMode::ABSOLUTE_X}},


// Clear/set Flags
    {0x18, Instruction{InstFunc::CLC_PTR, AddressMode::IMPLIED}}, //"implied"

    {0x38, Instruction{InstFunc::SEC_PTR, AddressMode::IMPLIED}},

    {0xD8, Instruction{InstFunc::CLD_PTR, AddressMode::IMPLIED}},

    {0xF8, Instruction{InstFunc::SED_PTR, AddressMode::IMPLIED}},

    {0x78, Instruction{InstFunc::SEI_PTR, AddressMode::IMPLIED}},

    {0x58, Instruction{InstFunc::CLI_PTR, AddressMode::IMPLIED}},

    {0xB8, Instruction{InstFunc::CLV_PTR, AddressMode::IMPLIED}},

    {0x40, Instruction{InstFunc::RTI_PTR, AddressMode::IMPLIED}},


// JMP
    {0x4C, Instruction{InstFunc::JMP_PTR, AddressMode::ABSOLUTE}},
    {0x6C, Instruction{InstFunc::JMP_PTR, AddressMode::INDIRECT}},


// Compare
    {0xC9, Instruction{InstFunc::CMP_PTR, AddressMode::IMMEDIATE}},
    {0xC5, Instruction{InstFunc::CMP_PTR, AddressMode::ZERO_PAGE}},
    {0xD5, Instruction{InstFunc::CMP_PTR, AddressMode::ZERO_PAGE_X}},
    {0xCD, Instruction{InstFunc::CMP_PTR, AddressMode::ABSOLUTE}},
    {0xDD, Instruction{InstFunc::CMP_PTR, AddressMode::ABSOLUTE_X}},
    {0xD9, Instruction{InstFunc::CMP_PTR, AddressMode::ABSOLUTE_Y}},
    {0xC1, Instruction{InstFunc::CMP_PTR, AddressMode::INDIRECT_X}},
    {0xD1, Instruction{InstFunc::CMP_PTR, AddressMode::INDIRECT_Y}},

    {0xC0, Instruction{InstFunc::CPY_PTR, AddressMode::IMMEDIATE}},
    {0xC4, Instruction{InstFunc::CPY_PTR, AddressMode::ZERO_PAGE}},
    {0xCC, Instruction{InstFunc::CPY_PTR, AddressMode::ABSOLUTE}},

    {0xE0, Instruction{InstFunc::CPX_PTR, AddressMode::IMMEDIATE}},
    {0xE4, Instruction{InstFunc::CPX_PTR, AddressMode::ZERO_PAGE}},
    {0xEC, Instruction{InstFunc::CPX_PTR, AddressMode::ABSOLUTE}},

    {0x24, Instruction{InstFunc::BIT_PTR, AddressMode::ZERO_PAGE}},
    {0x2C, Instruction{InstFunc::BIT_PTR, AddressMode::ABSOLUTE}},



// Conditional Branching
    {0xF0, Instruction{InstFunc::BEQ_PTR, AddressMode::RELATIVE}},

    {0xD0, Instruction{InstFunc::BNE_PTR, AddressMode::RELATIVE}},

    {0x90, Instruction{InstFunc::BCC_PTR, AddressMode::RELATIVE}},

    {0xB0, Instruction{InstFunc::BCS_PTR, AddressMode::RELATIVE}},

    {0x10, Instruction{InstFunc::BPL_PTR, AddressMode::RELATIVE}},

    {0x30, Instruction{InstFunc::BMI_PTR, AddressMode::RELATIVE}},

    {0x50, Instruction{InstFunc::BVC_PTR, AddressMode::RELATIVE}},

    {0x70, Instruction{InstFunc::BVS_PTR, AddressMode::RELATIVE}},


// Subroutine
    {0x20, Instruction{InstFunc::JSR_PTR, AddressMode::ABSOLUTE}},

    {0x60, Instruction{InstFunc::RTS_PTR, AddressMode::IMPLIED}},


// Pull register from stack
    {0x28, Instruction{InstFunc::PLP_PTR, AddressMode::IMPLIED}},

    {0x08, Instruction{InstFunc::PHP_PTR, AddressMode::IMPLIED}},

    {0x68, Instruction{InstFunc::PLA_PTR, AddressMode::IMPLIED}},

    {0x48, Instruction{InstFunc::PHA_PTR, AddressMode::IMPLIED}},


// Transfer
    {0xAA, Instruction{InstFunc::TAX_PTR, AddressMode::IMPLIED}},

    {0x8A, Instruction{InstFunc::TXA_PTR, AddressMode::IMPLIED}},

    {0x9A, Instruction{InstFunc::TXS_PTR, AddressMode::IMPLIED}},

    {0xBA, Instruction{InstFunc::TSX_PTR, AddressMode::IMPLIED}},

    {0x98, Instruction{InstFunc::TYA_PTR, AddressMode::IMPLIED}},

    {0xA8, Instruction{InstFunc::TAY_PTR, AddressMode::IMPLIED}},

    {0x00, Instruction{InstFunc::BRK_PTR, AddressMode::IMPLIED}},
    {0xea, Instruction{InstFunc::NOP_PTR, AddressMode::IMPLIED}}
};

const Instruction& GetInstruction(const uint8_t &opcode)
{
    return INSTRUCTION_MAP.at(opcode);
}
const bool InstructionValid(const uint8_t &opcode)
{

    return INSTRUCTION_MAP.find(opcode) != INSTRUCTION_MAP.end();
}