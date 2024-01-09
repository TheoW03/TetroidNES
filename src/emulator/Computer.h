#include <iostream>

using namespace std;
#ifndef CPU_PROCESSOR
#define CPU_PROCESSOR
struct CPUProcessor
{
    uint8_t X_Reg;
    uint8_t Y_Reg;
    uint8_t A_Reg;
    uint16_t PC;
    uint8_t status;
};
#endif
void run();