#include <map>
// #include "../emulator/Computer.h"
#include <Emulator/Instructions.h>

// #include "../emulator/Computer.h"
using instructionPointer = void (*)(AddressMode, CPU &);

struct Instruction
{
    instructionPointer i;
    AddressMode addressmode;
};

void initializeInstructionMap();
// Instruction initializeInstructionMap();
Instruction& GetInstruction(const uint8_t &opcode);
const bool InstructionValid(const uint8_t &opcode);