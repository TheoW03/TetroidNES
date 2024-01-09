#include "Computer.h"
#include <iostream>

using namespace std;
void set_zero(uint8_t value, CPUProcessor &cpu);
int check_zero(CPUProcessor &cpu);
void set_negative(uint8_t value, CPUProcessor &cpu);
int check_negative(CPUProcessor &cpu);
void set_brk(CPUProcessor &cpu);
int check_brk(CPUProcessor &cpu);
void set_interrupt_disabled(int isDisabled, CPUProcessor &cpu);
int check_Interrupt_disabled(CPUProcessor &cpu);