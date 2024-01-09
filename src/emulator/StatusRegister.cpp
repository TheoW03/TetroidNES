#include <iostream>

#include "Computer.h"
using namespace std;

int a = 0b1111111;
#define CARRY_BIT 0b0000001
#define ZERO_BIT 0b0000010
#define INTERRUPT_DISABLED_BIT 0b0000100
#define DECIMAL_BIT 0b0001000
#define BREAK 0b0010000
#define OVERFLOW 0b0100000
#define NEGATIVE 0b1000000
void set_zero(uint8_t value, CPUProcessor &cpu)
{
    if (value == 0)
    {
        cpu.status = cpu.status | ZERO_BIT;
    }
    else
    {
        cpu.status = cpu.status & (~ZERO_BIT);
    }
}

int check_zero(CPUProcessor &cpu)
{
    return cpu.status & ZERO_BIT;
}

void set_negative(uint8_t value, CPUProcessor &cpu)
{

    if ((value & 0b10000000) != 0)
    {
        cpu.status = cpu.status | NEGATIVE;
    }
    else
    {
        cpu.status = cpu.status & (~NEGATIVE);
    }
}

int check_negative(CPUProcessor &cpu)
{
    return cpu.status & NEGATIVE;
}

void set_brk(CPUProcessor &cpu)
{
    cpu.status = cpu.status | BREAK;
}

int check_brk(CPUProcessor &cpu)
{
    return cpu.status & BREAK;
}

void set_interrupt_disabled(int isDisabled, CPUProcessor &cpu)
{
    if (isDisabled == 1)
    {
        cpu.status = cpu.status | INTERRUPT_DISABLED_BIT;
    }
    else
    {
        cpu.status = cpu.status & (~INTERRUPT_DISABLED_BIT);
    }
}

int check_Interrupt_disabled(CPUProcessor &cpu)
{
    return cpu.status & INTERRUPT_DISABLED_BIT;
}