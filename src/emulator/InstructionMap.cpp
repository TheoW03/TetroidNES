#include <map>
// #include "../emulator/Computer.h"
#include "../emulator/Instructions.h"
#include <stdint.h>
using instructionPointer = void (*)(AddressMode, CPU &);

struct Instruction
{
    instructionPointer i;
    AddressMode addressmode;
};

std::map<uint8_t, Instruction> instructionMap;

void initializeInstructionMap()
{

    using std::make_pair; // here bc im lazy you may use using on the stack level. but lets stick to this

#pragma region LDA
    instructionMap.insert(make_pair(0xA9, Instruction{(instructionPointer)LDA, AddressMode::IMMEDIATE}));
    instructionMap.insert(make_pair(0xA5, Instruction{(instructionPointer)LDA, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0xB5, Instruction{(instructionPointer)LDA, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0xAD, Instruction{(instructionPointer)LDA, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0xBD, Instruction{(instructionPointer)LDA, AddressMode::ABSOLUTE_X}));
    instructionMap.insert(make_pair(0xB9, Instruction{(instructionPointer)LDA, AddressMode::ABSOLUTE_Y}));
    instructionMap.insert(make_pair(0xA1, Instruction{(instructionPointer)LDA, AddressMode::INDIRECT_X}));
    instructionMap.insert(make_pair(0xB1, Instruction{(instructionPointer)LDA, AddressMode::INDIRECT_Y}));
#pragma endregion

#pragma region LDX

    instructionMap.insert(make_pair(0xA2, Instruction{(instructionPointer)LDX, AddressMode::IMMEDIATE}));
    instructionMap.insert(make_pair(0xA6, Instruction{(instructionPointer)LDX, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0xB6, Instruction{(instructionPointer)LDX, AddressMode::ZERO_PAGE_Y}));
    instructionMap.insert(make_pair(0xAE, Instruction{(instructionPointer)LDX, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0xBE, Instruction{(instructionPointer)LDX, AddressMode::ABSOLUTE_Y}));

#pragma endregion

#pragma region LDY

    instructionMap.insert(make_pair(0xA0, Instruction{(instructionPointer)LDY, AddressMode::IMMEDIATE}));
    instructionMap.insert(make_pair(0xA4, Instruction{(instructionPointer)LDY, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0xB4, Instruction{(instructionPointer)LDY, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0xAC, Instruction{(instructionPointer)LDY, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0xBC, Instruction{(instructionPointer)LDY, AddressMode::ABSOLUTE_X}));

#pragma endregion

#pragma region ADC
    instructionMap.insert(make_pair(0x69, Instruction{(instructionPointer)ADC, AddressMode::IMMEDIATE}));
    instructionMap.insert(make_pair(0x65, Instruction{(instructionPointer)ADC, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0x75, Instruction{(instructionPointer)ADC, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0x6D, Instruction{(instructionPointer)ADC, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0x7D, Instruction{(instructionPointer)ADC, AddressMode::ABSOLUTE_X}));
    instructionMap.insert(make_pair(0x79, Instruction{(instructionPointer)ADC, AddressMode::ABSOLUTE_Y}));
    instructionMap.insert(make_pair(0x71, Instruction{(instructionPointer)ADC, AddressMode::INDIRECT_X}));
    instructionMap.insert(make_pair(0x61, Instruction{(instructionPointer)ADC, AddressMode::INDIRECT_Y}));
#pragma endregion

#pragma region SBC
    instructionMap.insert(make_pair(0xE9, Instruction{(instructionPointer)SBC, AddressMode::IMMEDIATE}));
    instructionMap.insert(make_pair(0xE5, Instruction{(instructionPointer)SBC, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0xF5, Instruction{(instructionPointer)SBC, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0xED, Instruction{(instructionPointer)SBC, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0xFD, Instruction{(instructionPointer)SBC, AddressMode::ABSOLUTE_X}));
    instructionMap.insert(make_pair(0xF9, Instruction{(instructionPointer)SBC, AddressMode::ABSOLUTE_Y}));
    instructionMap.insert(make_pair(0xF1, Instruction{(instructionPointer)SBC, AddressMode::INDIRECT_X}));
    instructionMap.insert(make_pair(0xE1, Instruction{(instructionPointer)SBC, AddressMode::INDIRECT_Y}));
#pragma endregion

#pragma region AND
    instructionMap.insert(make_pair(0x29, Instruction{(instructionPointer)AND, AddressMode::IMMEDIATE}));
    instructionMap.insert(make_pair(0x25, Instruction{(instructionPointer)AND, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0x35, Instruction{(instructionPointer)AND, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0x2D, Instruction{(instructionPointer)AND, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0x3D, Instruction{(instructionPointer)AND, AddressMode::ABSOLUTE_X}));
    instructionMap.insert(make_pair(0x39, Instruction{(instructionPointer)AND, AddressMode::ABSOLUTE_Y}));
    instructionMap.insert(make_pair(0x21, Instruction{(instructionPointer)AND, AddressMode::INDIRECT_X}));
    instructionMap.insert(make_pair(0x31, Instruction{(instructionPointer)AND, AddressMode::INDIRECT_Y}));
#pragma endregion

#pragma region ORA
    instructionMap.insert(make_pair(0x09, Instruction{(instructionPointer)ORA, AddressMode::IMMEDIATE}));
    instructionMap.insert(make_pair(0x05, Instruction{(instructionPointer)ORA, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0x15, Instruction{(instructionPointer)ORA, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0x0D, Instruction{(instructionPointer)ORA, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0x1D, Instruction{(instructionPointer)ORA, AddressMode::ABSOLUTE_X}));
    instructionMap.insert(make_pair(0x19, Instruction{(instructionPointer)ORA, AddressMode::ABSOLUTE_Y}));
    instructionMap.insert(make_pair(0x01, Instruction{(instructionPointer)ORA, AddressMode::INDIRECT_X}));
    instructionMap.insert(make_pair(0x11, Instruction{(instructionPointer)ORA, AddressMode::INDIRECT_Y}));
#pragma endregion

#pragma region ROL/ROR
    instructionMap.insert(make_pair(0x2A, Instruction{(instructionPointer)ROL, AddressMode::ACCUMULATOR}));
    instructionMap.insert(make_pair(0x26, Instruction{(instructionPointer)ROL, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0x36, Instruction{(instructionPointer)ROL, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0x2E, Instruction{(instructionPointer)ROL, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0x3E, Instruction{(instructionPointer)ROL, AddressMode::ABSOLUTE_X}));

    instructionMap.insert(make_pair(0x6A, Instruction{(instructionPointer)ROR, AddressMode::ACCUMULATOR}));
    instructionMap.insert(make_pair(0x66, Instruction{(instructionPointer)ROR, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0x76, Instruction{(instructionPointer)ROR, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0x6E, Instruction{(instructionPointer)ROR, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0x7E, Instruction{(instructionPointer)ROR, AddressMode::ABSOLUTE_X}));
#pragma endregion

#pragma region STX
    instructionMap.insert(make_pair(0x86, Instruction{(instructionPointer)STX, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0x96, Instruction{(instructionPointer)STX, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0x8E, Instruction{(instructionPointer)STX, AddressMode::ABSOLUTE}));

    instructionMap.insert(make_pair(0x84, Instruction{(instructionPointer)STY, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0x94, Instruction{(instructionPointer)STY, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0x8C, Instruction{(instructionPointer)STY, AddressMode::ZERO_PAGE_Y}));

    instructionMap.insert(make_pair(0x85, Instruction{(instructionPointer)STA, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0x95, Instruction{(instructionPointer)STA, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0x8D, Instruction{(instructionPointer)STA, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0x9D, Instruction{(instructionPointer)STA, AddressMode::ABSOLUTE_X}));
    instructionMap.insert(make_pair(0x99, Instruction{(instructionPointer)STA, AddressMode::ABSOLUTE_Y}));
    instructionMap.insert(make_pair(0x81, Instruction{(instructionPointer)STA, AddressMode::INDIRECT_X}));
    instructionMap.insert(make_pair(0x91, Instruction{(instructionPointer)STA, AddressMode::INDIRECT_Y}));
#pragma endregion

#pragma region bitshift
    instructionMap.insert(make_pair(0x4A, Instruction{(instructionPointer)LSR, AddressMode::ACCUMULATOR}));
    instructionMap.insert(make_pair(0x46, Instruction{(instructionPointer)LSR, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0x56, Instruction{(instructionPointer)LSR, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0x4E, Instruction{(instructionPointer)LSR, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0x5E, Instruction{(instructionPointer)LSR, AddressMode::ABSOLUTE_X}));

    instructionMap.insert(make_pair(0x0A, Instruction{(instructionPointer)ASL, AddressMode::ACCUMULATOR}));
    instructionMap.insert(make_pair(0x06, Instruction{(instructionPointer)ASL, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0x16, Instruction{(instructionPointer)ASL, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0x0E, Instruction{(instructionPointer)ASL, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0x1E, Instruction{(instructionPointer)ASL, AddressMode::ABSOLUTE_X}));
#pragma endregion

#pragma region XOR
    instructionMap.insert(make_pair(0x49, Instruction{(instructionPointer)EOR, AddressMode::IMMEDIATE}));
    instructionMap.insert(make_pair(0x45, Instruction{(instructionPointer)EOR, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0x55, Instruction{(instructionPointer)EOR, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0x4D, Instruction{(instructionPointer)EOR, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0x5D, Instruction{(instructionPointer)EOR, AddressMode::ABSOLUTE_X}));
    instructionMap.insert(make_pair(0x59, Instruction{(instructionPointer)EOR, AddressMode::ABSOLUTE_Y}));
    instructionMap.insert(make_pair(0x41, Instruction{(instructionPointer)EOR, AddressMode::INDIRECT_X}));
    instructionMap.insert(make_pair(0x51, Instruction{(instructionPointer)EOR, AddressMode::INDIRECT_Y}));
#pragma endregion

#pragma region DEC
    instructionMap.insert(make_pair(0xCA, Instruction{(instructionPointer)DEX, AddressMode::IMPLIED}));
    instructionMap.insert(make_pair(0x88, Instruction{(instructionPointer)DEY, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0xC6, Instruction{(instructionPointer)DEC, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0xD6, Instruction{(instructionPointer)DEC, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0xCE, Instruction{(instructionPointer)DEC, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0xDE, Instruction{(instructionPointer)DEC, AddressMode::ABSOLUTE_X}));
#pragma endregion

#pragma region INC
    instructionMap.insert(make_pair(0xE8, Instruction{(instructionPointer)INX, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0xC8, Instruction{(instructionPointer)INY, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0xE6, Instruction{(instructionPointer)INC, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0xF6, Instruction{(instructionPointer)INC, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0xEE, Instruction{(instructionPointer)INC, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0xFE, Instruction{(instructionPointer)INC, AddressMode::ABSOLUTE_X}));
#pragma endregion

#pragma region Clear/set Flags
    instructionMap.insert(make_pair(0x18, Instruction{(instructionPointer)CLC, AddressMode::IMPLIED})); //"implied"

    instructionMap.insert(make_pair(0x38, Instruction{(instructionPointer)SEC, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0xD8, Instruction{(instructionPointer)CLD, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0xF8, Instruction{(instructionPointer)SED, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0x78, Instruction{(instructionPointer)SEI, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0x58, Instruction{(instructionPointer)CLI, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0xB8, Instruction{(instructionPointer)CLV, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0x40, Instruction{(instructionPointer)RTI, AddressMode::IMPLIED}));
#pragma endregion

#pragma region JMP
    instructionMap.insert(make_pair(0x4C, Instruction{(instructionPointer)JMP, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0x6C, Instruction{(instructionPointer)JMP, AddressMode::INDIRECT}));
#pragma endregion

#pragma region Compare
    instructionMap.insert(make_pair(0xC9, Instruction{(instructionPointer)CMP, AddressMode::IMMEDIATE}));
    instructionMap.insert(make_pair(0xC5, Instruction{(instructionPointer)CMP, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0xD5, Instruction{(instructionPointer)CMP, AddressMode::ZERO_PAGE_X}));
    instructionMap.insert(make_pair(0xCD, Instruction{(instructionPointer)CMP, AddressMode::ABSOLUTE}));
    instructionMap.insert(make_pair(0xDD, Instruction{(instructionPointer)CMP, AddressMode::ABSOLUTE_X}));
    instructionMap.insert(make_pair(0xD9, Instruction{(instructionPointer)CMP, AddressMode::ABSOLUTE_Y}));
    instructionMap.insert(make_pair(0xC1, Instruction{(instructionPointer)CMP, AddressMode::INDIRECT_X}));
    instructionMap.insert(make_pair(0xD1, Instruction{(instructionPointer)CMP, AddressMode::INDIRECT_Y}));

    instructionMap.insert(make_pair(0xC0, Instruction{(instructionPointer)CPY, AddressMode::IMMEDIATE}));
    instructionMap.insert(make_pair(0xC4, Instruction{(instructionPointer)CPY, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0xCC, Instruction{(instructionPointer)CPY, AddressMode::ABSOLUTE}));

    instructionMap.insert(make_pair(0xE0, Instruction{(instructionPointer)CPX, AddressMode::IMMEDIATE}));
    instructionMap.insert(make_pair(0xE4, Instruction{(instructionPointer)CPX, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0xEC, Instruction{(instructionPointer)CPX, AddressMode::ABSOLUTE}));

    instructionMap.insert(make_pair(0x24, Instruction{(instructionPointer)BIT, AddressMode::ZERO_PAGE}));
    instructionMap.insert(make_pair(0x2C, Instruction{(instructionPointer)BIT, AddressMode::ABSOLUTE}));

#pragma endregion

#pragma region Conditional Branching
    instructionMap.insert(make_pair(0xF0, Instruction{(instructionPointer)BEQ, AddressMode::RELATIVE}));

    instructionMap.insert(make_pair(0xD0, Instruction{(instructionPointer)BNE, AddressMode::RELATIVE}));

    instructionMap.insert(make_pair(0x90, Instruction{(instructionPointer)BCC, AddressMode::RELATIVE}));

    instructionMap.insert(make_pair(0xB0, Instruction{(instructionPointer)BCS, AddressMode::RELATIVE}));

    instructionMap.insert(make_pair(0x10, Instruction{(instructionPointer)BPL, AddressMode::RELATIVE}));

    instructionMap.insert(make_pair(0x30, Instruction{(instructionPointer)BMI, AddressMode::RELATIVE}));

    instructionMap.insert(make_pair(0x50, Instruction{(instructionPointer)BVC, AddressMode::RELATIVE}));

    instructionMap.insert(make_pair(0x70, Instruction{(instructionPointer)BVS, AddressMode::RELATIVE}));
#pragma endregion

#pragma region Subroutine
    instructionMap.insert(make_pair(0x20, Instruction{(instructionPointer)JSR, AddressMode::ABSOLUTE}));

    instructionMap.insert(make_pair(0x60, Instruction{(instructionPointer)RTS, AddressMode::IMPLIED}));
#pragma endregion

#pragma region Pull register from stack
    instructionMap.insert(make_pair(0x28, Instruction{(instructionPointer)PLP, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0x08, Instruction{(instructionPointer)PHP, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0x68, Instruction{(instructionPointer)PLA, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0x48, Instruction{(instructionPointer)PHA, AddressMode::IMPLIED}));
#pragma endregion

#pragma region Transfer
    instructionMap.insert(make_pair(0xAA, Instruction{(instructionPointer)TAX, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0x8A, Instruction{(instructionPointer)TXA, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0x9A, Instruction{(instructionPointer)TXS, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0xBA, Instruction{(instructionPointer)TSX, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0x98, Instruction{(instructionPointer)TYA, AddressMode::IMPLIED}));

    instructionMap.insert(make_pair(0xA8, Instruction{(instructionPointer)TAY, AddressMode::IMPLIED}));
#pragma endregion
    instructionMap.insert(make_pair(0x00, Instruction{(instructionPointer)BRK, AddressMode::IMPLIED}));
    instructionMap.insert(make_pair(0xea, Instruction{(instructionPointer)NOP, AddressMode::IMPLIED}));
}
Instruction GetInstruction(uint8_t opcode)
{
    return instructionMap.at(opcode);
}
bool InstructionValid(uint8_t opcode)
{
    return instructionMap.find(opcode) != instructionMap.end();
}