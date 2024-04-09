#include <iostream>
#include "Bus.h"
#include "Computer.h"
using namespace std;

Bus addressBus = Bus();

uint16_t immediate_AddressMode(CPU &cpu)
{
    return cpu.PC;
}
uint16_t Zero_page_AddressMode(CPU &cpu)
{
    return (uint16_t)cpu.bus.read_8bit(cpu.PC);
}
uint16_t Zero_page_AddressMode_X(CPU &cpu)
{
    return (cpu.bus.read_8bit(cpu.PC) + cpu.X_Reg);
}
uint16_t Zero_page_AddressMode_Y(CPU&cpu)
{
    return ((cpu.bus.read_8bit(cpu.PC) + cpu.Y_Reg));
}

uint16_t Absolute(CPU &cpu)
{
    // printf("%x \n", read_16bit(PC));
    return (cpu.bus.read_16bit(cpu.PC));
}

uint16_t Absolute_page_AddressMode_X(CPU &cpu)
{
    return (cpu.bus.read_16bit(cpu.PC) + cpu.X_Reg);
}
uint16_t Absolute_page_AddressMode_Y(CPU &cpu)
{

    return (addressBus.read_16bit(cpu.PC) + cpu.Y_Reg);
}
uint16_t Indirect_AddressMode(CPU &cpu)
{
    uint16_t value = (uint16_t)cpu.bus.read_8bit(cpu.PC);
    return cpu.bus.read_16bit(value);
}
uint16_t Indirect_AddressModeX(CPU &cpu)
{
    return cpu.bus.read_16bit((cpu.bus.read_8bit(cpu.PC) + cpu.X_Reg));
}
uint16_t Indirect_AddressModeY(CPU &cpu)
{
    return (cpu.bus.read_16bit(cpu.bus.read_8bit(cpu.PC)) + cpu.Y_Reg);
}