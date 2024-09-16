#include "Computer.h"
#include <iostream>

void set_zero(uint8_t value, CPU &cpu);
int check_zero(CPU &cpu);
void set_negative(uint8_t value, CPU &cpu);
int check_negative(CPU &cpu);
void set_brk(CPU &cpu, int brk);
int check_brk(CPU &cpu);
void set_interrupt_disabled(int isDisabled, CPU &cpu);
int check_interrupt_disabled(CPU &cpu);
void set_carry(int isCarry, CPU &cpu);
int check_carry(CPU &cpu);
void set_overflow(uint8_t c_in, uint8_t c_out, CPU &cpu);

void set_decimal_mode(int isDecimal, CPU &cpu);
int check_decimal(CPU &cpu);
void set_overflow(int overflow, CPU &cpu);
int check_overflow(CPU &cpu);