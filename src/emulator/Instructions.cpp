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

#pragma region General purpose
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
	address_Mode_map[0x69] = AddressMode::IMMEDIATE; // nice
	address_Mode_map[0x65] = AddressMode::ZERO_PAGE;
	address_Mode_map[0x75] = AddressMode::ZERO_PAGE_X;
	address_Mode_map[0x6D] = AddressMode::ABSOLUTE;
	address_Mode_map[0x7D] = AddressMode::ABSOLUTE_X;
	address_Mode_map[0x79] = AddressMode::ABSOLUTE_Y;
	address_Mode_map[0x61] = AddressMode::INDIRECT_X;
	address_Mode_map[0x71] = AddressMode::INDIRECT_Y;
	uint8_t carry = 0;
	uint8_t value = read_8bit(address_Mode(address_Mode_map[current_instruction],
										   cpu.PC, cpu.X_Reg, cpu.Y_Reg));
	cpu.A_Reg = add(cpu.A_Reg, value, cpu, carry);
	set_carry(carry, cpu);
	set_zero(cpu.A_Reg, cpu);
	set_negative(cpu.A_Reg, cpu);
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE		// meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X // meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_Y)
	{
		cpu.PC++;
	}
	if (check_carry(cpu) != 0)
		cpu.A_Reg++;
	cpu.PC++;
}
void SBC(uint8_t current_instruction, CPUProcessor &cpu)
{
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0xE9] = AddressMode::IMMEDIATE; // meow :3
	address_Mode_map[0xE5] = AddressMode::ZERO_PAGE;
	address_Mode_map[0xF5] = AddressMode::ZERO_PAGE_X;
	address_Mode_map[0xED] = AddressMode::ABSOLUTE;
	address_Mode_map[0xFD] = AddressMode::ABSOLUTE_X;
	address_Mode_map[0xF9] = AddressMode::ABSOLUTE_Y;
	address_Mode_map[0xE1] = AddressMode::INDIRECT_X;
	address_Mode_map[0xF1] = AddressMode::INDIRECT_Y;
	uint8_t carry = 0;
	uint8_t value = read_8bit(address_Mode(address_Mode_map[current_instruction],
										   cpu.PC, cpu.X_Reg, cpu.Y_Reg));
	cpu.A_Reg = sub(cpu.A_Reg, value, cpu, carry);

	set_carry(carry, cpu);
	set_zero(cpu.A_Reg, cpu);
	set_negative(cpu.A_Reg, cpu);
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE		// meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X // meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_Y)
	{
		cpu.PC++;
	}
	cpu.A_Reg -= 1 - carry;
	cpu.PC++;
}
void BIT(uint8_t current_instruction, CPUProcessor &cpu)
{
	// TODO: bit test
}
void AND(uint8_t current_instruction, CPUProcessor &cpu)
{
	// TODO: and
}
void ORA(uint8_t current_instruction, CPUProcessor &cpu)
{
	// TODO: or
}
void ROR(uint8_t current_instruction, CPUProcessor &cpu)
{
	// TODO: rotate right
}
void ROL(uint8_t current_instruction, CPUProcessor &cpu)
{
	// TODO: rotate left
}
#pragma endregion
// flag setting instructions

#pragma region setFlags
void CLC(uint8_t current_instruction, CPUProcessor &cpu)
{
	set_carry(0, cpu);
}
void SEC(uint8_t current_instruction, CPUProcessor &cpu)
{
	set_carry(1, cpu);
}
#pragma endregion
// JMP instructiions

#pragma region Jmp instructions
void JMP(uint8_t current_instruction, CPUProcessor &cpu)
{

	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0x4C] = AddressMode::ABSOLUTE;
	address_Mode_map[0x6C] = AddressMode::INDIRECT;
	uint16_t new_PC = address_Mode(address_Mode_map[current_instruction],
								   cpu.PC, cpu.X_Reg, cpu.Y_Reg);
	cpu.PC = (new_PC + 0x8000);
}
void BEQ(uint8_t current_instruction, CPUProcessor &cpu)
{
	if (check_zero(cpu) == 0)
	{
		cpu.PC++;
		return;
	}
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0xF0] = AddressMode::IMMEDIATE;
	int16_t new_PC = (int16_t)read_8bit(address_Mode(address_Mode_map[current_instruction],
													 cpu.PC, cpu.X_Reg, cpu.Y_Reg));
	cpu.PC = (new_PC + 0x8000);
}
void BNE(uint8_t current_instruction, CPUProcessor &cpu)
{
	if (check_zero(cpu) != 0)
	{
		cpu.PC++;
		return;
	}
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0xD0] = AddressMode::IMMEDIATE;
	int16_t new_PC = (int16_t)read_8bit(address_Mode(address_Mode_map[current_instruction],
													 cpu.PC, cpu.X_Reg, cpu.Y_Reg));
	// cpu.PC++;
	cpu.PC = (new_PC + 0x8000);
}
void BCC(uint8_t current_instruction, CPUProcessor &cpu)
{
	// TODO: Branch if carry clear
}
void BCS(uint8_t current_instruction, CPUProcessor &cpu)
{
	// TODO: Branch if carry set
}
void BPL(uint8_t current_instruction, CPUProcessor &cpu)
{
	// TODO: Branch if ~Negat
}
void BVC(uint8_t current_instruction, CPUProcessor &cpu)
{
	// TODO: Branch if overflow
}
void JSR(uint8_t current_instruction, CPUProcessor &cpu)
{
	// TODO: functions
}
void CMP(uint8_t current_instruction, CPUProcessor &cpu)
{
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0xC9] = AddressMode::IMMEDIATE; // meow :3
	address_Mode_map[0xC5] = AddressMode::ZERO_PAGE;
	address_Mode_map[0xD5] = AddressMode::ZERO_PAGE_X;
	address_Mode_map[0xCD] = AddressMode::ABSOLUTE;
	address_Mode_map[0xDD] = AddressMode::ABSOLUTE_X;
	address_Mode_map[0xD9] = AddressMode::ABSOLUTE_Y;
	address_Mode_map[0xC1] = AddressMode::INDIRECT_X;
	address_Mode_map[0xD1] = AddressMode::INDIRECT_Y;
	uint8_t carry = 0;
	uint8_t value = read_8bit(address_Mode(address_Mode_map[current_instruction],
										   cpu.PC, cpu.X_Reg, cpu.Y_Reg));
	uint8_t v = sub(cpu.A_Reg, value, cpu, carry);
	// printf("X_Reg: %d \n", v);
	// printf("carry: %d \n", carry);
	set_carry(carry, cpu);
	set_zero(v, cpu);
	set_negative(v, cpu);
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE		// meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X // meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_Y)
	{
		cpu.PC++;
	}
	cpu.PC++;
}

#pragma endregion