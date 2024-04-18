#include <iostream>
#include "Bus.h"
using namespace std;
#ifndef CPU_PROCESSOR
#define CPU_PROCESSOR
struct CPU
{
    uint8_t X_Reg;
    uint8_t Y_Reg;
    uint8_t A_Reg;
    uint16_t stack_pointer;
    uint16_t PC;
    uint8_t status;
    Bus bus;
};
#endif
void init(string file_name);