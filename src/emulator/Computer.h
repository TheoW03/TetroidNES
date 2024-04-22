#include <iostream>
#include "Bus.h"
using namespace std;
#ifndef CPU_H
#define CPU_H
struct CPU
{
    uint8_t X_Reg;
    uint8_t Y_Reg;
    uint8_t A_Reg;
    uint8_t status;
    Bus bus;
};
#endif
#ifndef ADDRESS_MODE
#define ADDRESS_MODE
enum class AddressMode
{
    ACCUMULATOR,
    ABSOLUTE,
    ABSOLUTE_Y,
    ABSOLUTE_X,
    ZERO_PAGE,
    ZERO_PAGE_X,
    ZERO_PAGE_Y,
    IMMEDIATE,
    INDIRECT,
    INDIRECT_X,
    INDIRECT_Y,
    RELATIVE,
    IMPLIED
};
#endif
void init(string file_name);