#include <iostream>
#include <bitset>
#include "Computer.h"
using namespace std;

#define CARRY_BIT 0b0000001
#define ZERO_BIT 0b0000010
#define INTERRUPT_DISABLED_BIT 0b0000100
#define DECIMAL_BIT 0b0001000
#define BREAK_BIT 0b0010000
#define OVERFLOW_BIT 0b0100000
#define NEGATIVE_BIT 0b1000000

void set_carry(int isCarry, CPUProcessor &cpu)
{
    if (isCarry != 0)
    {
        cpu.status = cpu.status | CARRY_BIT;
    }
    else
    {
        cpu.status = cpu.status & (~CARRY_BIT);
    }
}
int check_carry(CPUProcessor &cpu)
{
    return cpu.status & CARRY_BIT;
}
// void set_carry_test(uint8_t )
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
        cpu.status = cpu.status | NEGATIVE_BIT;
    }
    else
    {
        cpu.status = cpu.status & (~NEGATIVE_BIT);
    }
}

int check_negative(CPUProcessor &cpu)
{
    return cpu.status & NEGATIVE_BIT;
}

void set_brk(CPUProcessor &cpu)
{
    cpu.status = cpu.status | BREAK_BIT;
}

int check_brk(CPUProcessor &cpu)
{
    return cpu.status & BREAK_BIT;
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
void set_overflow(uint8_t c_in, uint8_t c_out, CPUProcessor &cpu)
{
    // cout << ((c_out & 0b10000000) & 0) << endl;
    
    if (((c_out & 0b10000000) != 0) != ((c_in & 0b01000000) != 0))
    {
        cpu.status = cpu.status | OVERFLOW_BIT;
        cout << "overflow" << endl;
    }
}
int check_overflow(CPUProcessor &cpu)
{
    return cpu.status & OVERFLOW_BIT;
}