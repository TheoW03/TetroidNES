#include <iostream>
#include <bitset>
#include "Computer.h"

#define CARRY_BIT 0b0000001
#define ZERO_BIT 0b0000010
#define INTERRUPT_DISABLED_BIT 0b0000100
#define DECIMAL_BIT 0b0001000
#define BREAK_BIT 0b0010000
#define OVERFLOW_BIT 0b0100000
#define NEGATIVE_BIT 0b1000000

void set_carry(int isCarry, CPU &cpu)
{
    if (isCarry != 0)
    {
        cpu.status.C = 1;
    }
    else
    {
        cpu.status.C = 0;
    }
}

int check_carry(CPU &cpu)
{
    return cpu.status.C;
}

void set_zero(uint8_t value, CPU &cpu)
{
    if (value == 0)
    {
        cpu.status.Z = 1;
    }
    else
    {
        cpu.status.Z = 0;
    }
}

int check_zero(CPU &cpu)
{
    return cpu.status.Z;
}

void set_negative(uint8_t value, CPU &cpu)
{

    if ((value & 0b10000000) != 0)
    {
        cpu.status.N = 1;
    }
    else
    {
        cpu.status.N = 0;
    }
}

int check_negative(CPU &cpu)
{
    return cpu.status.N;
}

void set_brk(CPU &cpu, int brk)
{
    if (brk == 1)
        cpu.status.B = 1;
    else
        cpu.status.B = 0;
}

int check_brk(CPU &cpu)
{
    return cpu.status.B;
}

void set_interrupt_disabled(int isDisabled, CPU &cpu)
{
    if (isDisabled == 1)
    {
        cpu.status.I = 1;
    }
    else
    {
        cpu.status.I = 0;
    }
}

int check_interrupt_disabled(CPU &cpu)
{
    return cpu.status.I;
}

void set_overflow(uint8_t c_in, uint8_t c_out, CPU &cpu)
{
    // cout << ((c_out & 0b10000000) & 0) << endl;

    if (((c_out & 0b10000000) != 0) != ((c_in & 0b01000000) != 0))
    {
        cpu.status.V = 1;
    }
    else
    {
        cpu.status.V = 0;
    }
}

void set_overflow(int overflow, CPU &cpu)
{
    // if (overflow == 1)
    // {
    //     cpu.status |= OVERFLOW_BIT;
    // }
    // else
    // {
    //     cpu.status &= (~OVERFLOW_BIT);
    // }
}

int check_overflow(CPU &cpu)
{
    return cpu.status.I;
}

void set_decimal_mode(int isDecimal, CPU &cpu)
{
    if (isDecimal == 1)
    {
        cpu.status.D = 1;
    }
    else
    {
        cpu.status.D = 0;
    }
}

int check_decimal(CPU &cpu)
{
    return cpu.status.D;
}
