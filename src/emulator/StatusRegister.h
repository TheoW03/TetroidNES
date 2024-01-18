#include "Computer.h"
#include <iostream>

using namespace std;
void set_zero(uint8_t value, CPUProcessor &cpu);
int check_zero(CPUProcessor &cpu);
void set_negative(uint8_t value, CPUProcessor &cpu);
int check_negative(CPUProcessor &cpu);
void set_brk(CPUProcessor &cpu, int brk);
int check_brk(CPUProcessor &cpu);
void set_interrupt_disabled(int isDisabled, CPUProcessor &cpu);
int check_Interrupt_disabled(CPUProcessor &cpu);
void set_carry(int isCarry, CPUProcessor &cpu);
int check_carry(CPUProcessor &cpu);
void set_overflow(uint8_t c_in, uint8_t c_out, CPUProcessor &cpu);

void set_decimal_mode(int isDecimal, CPUProcessor &cpu);
int check_decimal(CPUProcessor &cpu);
void set_overflow(int overflow, CPUProcessor &cpu);