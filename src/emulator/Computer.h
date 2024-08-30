#include <iostream>
#include "Bus.h"
#include <filesystem>

// #include <SFML/Graphics.hpp>
#ifndef CPU_H
#define CPU_H
struct CPU
{
    uint8_t X_Reg;
    uint8_t Y_Reg;
    uint8_t A_Reg;
    uint8_t status;
    union
    {
        struct
        {
            unsigned C : 1;
            unsigned Z : 1;
            unsigned I : 1;
            unsigned D : 1;
            unsigned B : 1;
            unsigned Unused : 1;
            unsigned O : 1;
            unsigned N : 1;

            // unsigned mapper_lower : 4; // lower bits of mapper
            // unsigned four_screen : 1;  // if 4 screen
            // unsigned trainer : 1;      // trainer. Ie the PRG is a 512 offset
            // unsigned battery : 1;      // for zelda (it used a battery to save )
            // unsigned vertical : 1;     // if vertical
        };
        uint8_t val;
    } cpu_status;

    // sf::RenderWindow Render_window;
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
