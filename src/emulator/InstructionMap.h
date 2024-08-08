#include <map>
// #include "../emulator/Computer.h"
#include "../emulator/Instructions.h"

// #include "../emulator/Computer.h"
using instructionPointer = void (*)(AddressMode, CPU &);

struct Instruction
{
    instructionPointer i;
    AddressMode addressmode;
};

void initializeInstructionMap();
// Instruction initializeInstructionMap();
Instruction GetInstruction(uint8_t opcode);
bool InstructionValid(uint8_t opcode);