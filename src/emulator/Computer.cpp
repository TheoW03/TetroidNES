#include <iostream>
#include "BitOperations.h"
#include "Memory.h"
#include "AddressMode.h"
#include "Instructions.h"
#include <cstdio>
#include <bitset>

using namespace std;

uint8_t X_REG = 0;
uint8_t Y_REG = 0;
uint8_t A_Reg = 0;
uint16_t PC = 0;
uint8_t status = 0;

uint8_t current_instruction = 0;
uint8_t param = 0;

void set_zero(uint8_t value)
{
	if (value == 0)
	{
		status = status | 0b00000010;
	}
	else
	{
		status = status & 0b11111101;
	}
}

int check_zero()
{
	return status & 0b00000010;
}

void set_negative(uint8_t value)
{
	if (value & 0b10000000 != 0)
	{
		status = status | 0b10000000;
	}
	else
	{
		status = status & 0b01111111;
	}
}

int check_negative()
{
	return status & 0b10000000;
}

void set_brk()
{
	status = status | 0b00100000;
}

int check_brk()
{
	return status & 0b00100000;
}

void set_interrupt_disabled(int isDisabled)
{
	if (isDisabled == 1)
	{
		status = status | 0b00001000;
	}
	else
	{
		status = status & 0b11110111;
	}
}

int check_Interrupt_disabled()
{
	return status & 0b00001000;
}
void run()
{
	while (1)
	{
		current_instruction = read_8bit(PC);

		PC++;
		switch (current_instruction)
		{
		case 0xA9:
		{
			param = read_8bit(immediate_AddressMode(PC));
			LDA(param, A_Reg);
			set_zero(A_Reg);
			set_negative(A_Reg);
			PC++;
		}
		case 0xA5:
		{
			param = read_8bit(Zero_page_AddressMode(PC));
			LDA(param, A_Reg);
			set_zero(A_Reg);
			set_negative(A_Reg);
			PC++;
		}
		case 0x00:
		{
			if (check_Interrupt_disabled() != 0)
				continue;
			printf("A_Reg: %d \n", A_Reg);
			printf("X_Reg: %d \n", X_REG);
			printf("Y_Reg: %d \n", Y_REG);
			printf("PC: %d \n", PC);
			set_brk();
			bitset<8> y(status);
			cout << "status: " << y << endl;
			return;
		}
		}
	}
}
