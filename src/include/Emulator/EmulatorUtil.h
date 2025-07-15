#pragma once

#include <optional>

#include <Emulator/Bus.h>

#define NES_RES_W 240
#define NES_RES_L 256
#define NES_RES_A 256 * 240
#define NES_RES_F 256 * 240 * 4

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
            unsigned D : 1; // Decimal mode (un-used in the NES)
            unsigned B : 1; // break
            unsigned Unused : 1;
            unsigned V : 1; // overflow
            unsigned N : 1; // negative
        };
        uint8_t val;
    } status;

    int error_code;

    Bus bus;
    std::optional<int> interrupt;
};