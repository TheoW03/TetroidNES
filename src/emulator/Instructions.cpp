#include <iostream>
#include "BitOperations.h"
#include "Memory.h"
#include "Computer.h"
#include "AddressMode.h"
#include <map>
#include "StatusRegister.h"
using namespace std;

enum class AddressMode
{
	ABSOLUTE,
	ABSOLUTE_Y,
	ABSOLUTE_X,
	ZERO_PAGE,
	ZERO_PAGE_X,
	ZERO_PAGE_Y,
	IMMEDIATE,
	INDIRECT,
	INDIRECT_X,
	INDIRECT_Y
};
uint16_t address_Mode(AddressMode address, uint16_t PC, uint8_t X_REG, uint16_t Y_REG)
{
	switch (address)
	{
	case AddressMode::IMMEDIATE:
	{
		return immediate_AddressMode(PC);
	}
	case AddressMode::ZERO_PAGE:
	{
		return Zero_page_AddressMode(PC);
	}
	case AddressMode::ZERO_PAGE_X:
	{
		return Zero_page_AddressMode_X(PC, X_REG);
	}
	case AddressMode::ZERO_PAGE_Y:
	{
		return Zero_page_AddressMode_Y(PC, Y_REG);
	}
	case AddressMode::ABSOLUTE:
	{
		return Absolute(PC);
	}
	case AddressMode::ABSOLUTE_X:
	{
		return Absolute_page_AddressMode_X(PC, X_REG);
	}
	case AddressMode::ABSOLUTE_Y:
	{
		return Absolute_page_AddressMode_Y(PC, Y_REG);
	}
	case AddressMode::INDIRECT:
	{
		return Indirect_AddressMode(PC);
	}
	case AddressMode::INDIRECT_X:
	{
		return Indirect_AddressModeX(PC, X_REG);
	}
	case AddressMode::INDIRECT_Y:
	{
		return Indirect_AddressModeY(PC, Y_REG);
	}
	default:
	{
		cout << "Instruction not supported" << endl;
		return 0;
	}
	}
}

void LDA(uint8_t current_instruction, CPUProcessor &cpu)
{
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0xA9] = AddressMode::IMMEDIATE;
	address_Mode_map[0xA5] = AddressMode::ZERO_PAGE;
	address_Mode_map[0xB5] = AddressMode::ZERO_PAGE_X;
	address_Mode_map[0xAD] = AddressMode::ABSOLUTE;
	address_Mode_map[0xBD] = AddressMode::ABSOLUTE_X;
	address_Mode_map[0xB9] = AddressMode::ABSOLUTE_Y;
	address_Mode_map[0xA1] = AddressMode::INDIRECT_X;
	address_Mode_map[0xB1] = AddressMode::INDIRECT_Y;
	uint8_t value = read_8bit(address_Mode(address_Mode_map[current_instruction],
										   cpu.PC, cpu.X_Reg, cpu.Y_Reg));
	cpu.A_Reg = value;
	cpu.PC++;
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE		// meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X // meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_Y)
	{
		cpu.PC++;
	}
}
void LDX(uint8_t current_instruction, CPUProcessor &cpu)
{
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0xA2] = AddressMode::IMMEDIATE;
	address_Mode_map[0xA6] = AddressMode::ZERO_PAGE;
	address_Mode_map[0xB6] = AddressMode::ZERO_PAGE_Y;
	address_Mode_map[0xAE] = AddressMode::ABSOLUTE;
	address_Mode_map[0xBE] = AddressMode::ABSOLUTE_Y;
	uint8_t value = read_8bit(address_Mode(address_Mode_map[current_instruction],
										   cpu.PC, cpu.X_Reg, cpu.Y_Reg));
	cpu.X_Reg = read_8bit(value);
	cpu.PC++;
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE // meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_Y)
	{
		cpu.PC++;
	}
	set_zero(cpu.X_Reg, cpu);
	set_negative(cpu.X_Reg, cpu);
}
void LDY(uint8_t current_instruction, CPUProcessor &cpu)
{
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0xA0] = AddressMode::IMMEDIATE;
	address_Mode_map[0xA4] = AddressMode::ZERO_PAGE;
	address_Mode_map[0xB4] = AddressMode::ZERO_PAGE_X;
	address_Mode_map[0xAC] = AddressMode::ABSOLUTE;
	address_Mode_map[0xBC] = AddressMode::ABSOLUTE_X;
	uint8_t value = read_8bit(address_Mode(address_Mode_map[current_instruction],
										   cpu.PC, cpu.X_Reg, cpu.Y_Reg));
	cpu.Y_Reg = read_8bit(value);
	cpu.PC++;
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE // meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X)
	{
		cpu.PC++;
	}
	set_zero(cpu.Y_Reg, cpu);
	set_negative(cpu.Y_Reg, cpu);
}
void ADC(uint8_t current_instruction, CPUProcessor &cpu)
{
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0x69] = AddressMode::IMMEDIATE; // nice :Smirk:
	uint8_t carry = 0;
	uint8_t value = read_8bit(address_Mode(address_Mode_map[current_instruction],
										   cpu.PC, cpu.X_Reg, cpu.Y_Reg));
	cpu.A_Reg = add(cpu.A_Reg, value, carry);

	set_carry(carry, cpu);
	set_zero(cpu.A_Reg, cpu);
	set_negative(cpu.A_Reg, cpu);
	cpu.PC++;
}
void SBC(uint8_t current_instruction, CPUProcessor &cpu)
{
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0xE9] = AddressMode::IMMEDIATE; // meow :3
	uint8_t carry = 0;
	uint8_t value = read_8bit(address_Mode(address_Mode_map[current_instruction],
										   cpu.PC, cpu.X_Reg, cpu.Y_Reg));
	cpu.A_Reg = sub(cpu.A_Reg, value, carry);

	set_carry(carry, cpu);
	set_zero(cpu.A_Reg, cpu);
	set_negative(cpu.A_Reg, cpu);
	cpu.PC++;
}