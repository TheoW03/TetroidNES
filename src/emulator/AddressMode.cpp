#include <iostream>
#include <cstdio>
#include "Bus.h"
#include "Computer.h"

using namespace std;

Bus addressBus = Bus();

uint8_t immediate_address_mode(CPU &cpu)
{
    return cpu.bus.fetch_next();
}

uint16_t zero_page_address_mode(CPU &cpu)
{
    return (uint16_t)cpu.bus.read_8bit(cpu.PC);
}

uint16_t zero_page_address_mode_X(CPU &cpu)
{
    return (cpu.bus.read_8bit(cpu.PC) + cpu.X_Reg);
}

uint16_t zero_page_address_mode_Y(CPU &cpu)
{
    return ((cpu.bus.read_8bit(cpu.PC) + cpu.Y_Reg));
}

uint16_t absolute(CPU &cpu)
{
    // printf("%x \n", read_16bit(PC));
    return (cpu.bus.read_16bit(cpu.PC));
}

uint16_t absolute_page_address_mode_X(CPU &cpu)
{
    return (cpu.bus.read_16bit(cpu.PC) + cpu.X_Reg);
}

uint16_t absolute_page_address_mode_Y(CPU &cpu)
{
    return (addressBus.read_16bit(cpu.PC) + cpu.Y_Reg);
}

uint16_t indirect_address_mode(CPU &cpu)
{
    uint16_t value = (uint16_t)cpu.bus.read_8bit(cpu.PC);
    return cpu.bus.read_16bit(value);
}

uint16_t indirect_address_mode_X(CPU &cpu)
{
    return cpu.bus.read_16bit((cpu.bus.read_8bit(cpu.PC) + cpu.X_Reg));
}

uint16_t indirect_address_Mode_Y(CPU &cpu)
{
    return (cpu.bus.read_16bit(cpu.bus.read_8bit(cpu.PC)) + cpu.Y_Reg);
}
