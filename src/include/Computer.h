#include <iostream>
#include "Bus.h"
#include <filesystem>

// #include <SFML/Graphics.hpp>
#ifndef CPU_H
#define CPU_H
struct CPU
{
    uint8_t A_Reg;
    uint8_t X_Reg;
    uint8_t Y_Reg;

    union
    {
        struct
        {
            unsigned C : 1; // Carry
            unsigned Z : 1; // Zero
            unsigned I : 1; // interrupt disabled
            unsigned D : 1; // Decimal mode (un-used)
            unsigned B : 1; // break
            unsigned Unused : 1;
            unsigned V : 1; // overflow
            unsigned N : 1; // negative
        };
        uint8_t val;
    } status;

    int error_code;

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
CPU run(CPU cpu, std::string file_name);
void printCPU_stats(CPU cpu);
CPU init(std::string file_name);
