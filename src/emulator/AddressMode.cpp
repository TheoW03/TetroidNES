#include <iostream>
#include <cstdio>
#include "Bus.h"
#include "Computer.h"

/**
 * @brief get PC because this is the address bus this. does the immediate address or
 * "return the PC"
 *
 * @param cpu
 * @return uint16_t
 */
uint16_t immediate_address_mode(CPU &cpu)
{
    return cpu.bus.get_PC();
}

/**
 * @brief zero page (imagine a ptr this is what this is. reads from a mem address)
 *
 * @param cpu
 * @return uint16_t
 */
uint16_t zero_page_address_mode(CPU &cpu)
{
    return cpu.bus.read_8bit(cpu.bus.get_PC());
}
/**
 * @brief relative address mode (used for conditional branching)
 *
 * @param cpu
 * @return uint16_t
 */
uint16_t relative_addressmode(CPU &cpu)
{
    return cpu.bus.get_PC();
}
/**
 * @brief 8 bit ptr offset X
 *
 * @param cpu
 * @return uint16_t
 */
uint16_t zero_page_address_mode_X(CPU &cpu)
{
    return cpu.bus.read_8bit(cpu.bus.get_PC()) + cpu.X_Reg;
}
/**
 * @brief 8 bit ptr offset Y
 *
 * @param cpu
 * @return uint16_t
 */
uint16_t zero_page_address_mode_Y(CPU &cpu)
{
    return cpu.bus.read_8bit(cpu.bus.get_PC()) + cpu.Y_Reg;
}

/**
 * @brief ptr but 16 bit
 *
 * @param cpu
 * @return uint16_t
 */
uint16_t absolute(CPU &cpu)
{

    return (cpu.bus.read_16bit(cpu.bus.get_PC()));
}
/**
 * @brief 16 bit ptr but offset with x
 *
 * @param cpu
 * @return uint16_t
 */
uint16_t absolute_page_address_mode_X(CPU &cpu)
{
    return (cpu.bus.read_16bit(cpu.bus.get_PC()) + cpu.X_Reg);
}
/**
 * @brief 16 bit ptr but offset Y
 *
 * @param cpu
 * @return uint16_t
 */
uint16_t absolute_page_address_mode_Y(CPU &cpu)
{
    return (cpu.bus.read_16bit(cpu.bus.get_PC()) + cpu.Y_Reg);
}

uint16_t indirect_address_mode(CPU &cpu)
{
    uint16_t value = (uint16_t)cpu.bus.read_8bit(cpu.bus.get_PC());
    return cpu.bus.read_16bit(value);
}

uint16_t indirect_address_mode_X(CPU &cpu)
{
    return cpu.bus.read_16bit((cpu.bus.read_8bit(cpu.bus.get_PC()) + cpu.X_Reg));
}

uint16_t indirect_address_Mode_Y(CPU &cpu)
{
    return (cpu.bus.read_16bit(cpu.bus.read_8bit(cpu.bus.get_PC())) + cpu.Y_Reg);
}
