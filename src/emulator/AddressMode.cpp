#include <iostream>
#include "Memory.h"
using namespace std;
uint16_t immediate_AddressMode(uint16_t PC)
{
    return PC;
}
uint16_t Zero_page_AddressMode(uint16_t PC)
{
    return (uint16_t)read_8bit(PC);
}
uint16_t Zero_page_AddressMode_X(uint16_t PC, uint8_t X_REG)
{
    return (read_8bit(PC) + X_REG) % 255;
}
uint16_t Zero_page_AddressMode_Y(uint16_t PC, uint8_t Y_REG)
{

    return (uint16_t)(read_8bit(PC) + Y_REG) % 255;
}

uint16_t Absolute(uint16_t PC)
{

    return (read_16bit(PC));
}

uint16_t Absolute_page_AddressMode_X(uint16_t PC, uint8_t X_REG)
{
    return (read_16bit(PC) + X_REG) % 65535;
}
uint16_t Absolute_page_AddressMode_Y(uint16_t PC, uint8_t Y_REG)
{

    return (read_16bit(PC) + Y_REG) % 65535;
}
