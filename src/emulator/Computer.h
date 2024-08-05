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
void init(std::string file_name);
void printCPU_stats(CPU cpu);
CPU test_init(std::string file_name);