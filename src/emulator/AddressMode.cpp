#include <iostream>
#include "Bus.h"
#include "Computer.h"
using namespace std;

Bus addressBus = Bus();

uint16_t immediate_AddressMode(CPUProcessor &cpu)
{
    return cpu.PC;
}
uint16_t Zero_page_AddressMode(CPUProcessor &cpu)
{
    return (uint16_t)cpu.bus.read_8bit(cpu.PC);
}
uint16_t Zero_page_AddressMode_X(CPUProcessor &cpu)
{
    return (cpu.bus.read_8bit(cpu.PC) + cpu.X_Reg);
}
uint16_t Zero_page_AddressMode_Y(CPUProcessor &cpu)
{
    return ((cpu.bus.read_8bit(cpu.PC) + cpu.Y_Reg));
}

uint16_t Absolute(CPUProcessor &cpu)
{
    // printf("%x \n", read_16bit(PC));
    return (cpu.bus.read_16bit(cpu.PC));
}

uint16_t Absolute_page_AddressMode_X(CPUProcessor &cpu)
{
    return (cpu.bus.read_16bit(cpu.PC) + cpu.X_Reg);
}
uint16_t Absolute_page_AddressMode_Y(CPUProcessor &cpu)
{

    return (addressBus.read_16bit(cpu.PC) + cpu.Y_Reg);
}
uint16_t Indirect_AddressMode(CPUProcessor &cpu)
{
    uint16_t value = (uint16_t)cpu.bus.read_8bit(cpu.PC);
    return cpu.bus.read_16bit(value);
}
uint16_t Indirect_AddressModeX(CPUProcessor &cpu)
{
    return cpu.bus.read_16bit((cpu.bus.read_8bit(cpu.PC) + cpu.X_Reg));
}
uint16_t Indirect_AddressModeY(CPUProcessor &cpu)
{
    return (cpu.bus.read_16bit(cpu.bus.read_8bit(cpu.PC)) + cpu.Y_Reg);
}