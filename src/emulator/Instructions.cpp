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

uint16_t address_Mode(AddressMode address, CPU &cpu)
{
	switch (address)
	{
	case AddressMode::IMMEDIATE:
	{
		return immediate_AddressMode(cpu);
	}
	case AddressMode::ZERO_PAGE:
	{
		return Zero_page_AddressMode(cpu);
	}
	case AddressMode::ZERO_PAGE_X:
	{
		return Zero_page_AddressMode_X(cpu);
	}
	case AddressMode::ZERO_PAGE_Y:
	{
		return Zero_page_AddressMode_Y(cpu);
	}
	case AddressMode::ABSOLUTE:
	{
		return Absolute(cpu);
	}
	case AddressMode::ABSOLUTE_X:
	{
		return Absolute_page_AddressMode_X(cpu);
	}
	case AddressMode::ABSOLUTE_Y:
	{
		return Absolute_page_AddressMode_Y(cpu);
	}
	case AddressMode::INDIRECT:
	{
		return Indirect_AddressMode(cpu);
	}
	case AddressMode::INDIRECT_X:
	{
		return Indirect_AddressModeX(cpu);
	}
	case AddressMode::INDIRECT_Y:
	{
		return Indirect_AddressModeY(cpu);
	}
	default:
	{
		cout << "Instruction not supported" << endl;
		return 0;
	}
	}
}

// data transfer instructions, includes MOV...Stack pointer what not
#pragma region data transfer instructions
void LDA(uint8_t current_instruction, CPU &cpu)
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
	uint8_t value = cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
												   cpu));
	cpu.A_Reg = value;
	cpu.PC++;
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE		// meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X // meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_Y)
	{
		cpu.PC++;
	}
	set_zero(cpu.A_Reg, cpu);
	set_negative(cpu.A_Reg, cpu);
}
void LDX(uint8_t current_instruction, CPU &cpu)
{
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0xA2] = AddressMode::IMMEDIATE;
	address_Mode_map[0xA6] = AddressMode::ZERO_PAGE;
	address_Mode_map[0xB6] = AddressMode::ZERO_PAGE_Y;
	address_Mode_map[0xAE] = AddressMode::ABSOLUTE;
	address_Mode_map[0xBE] = AddressMode::ABSOLUTE_Y;
	uint8_t value = cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
												   cpu));
	cpu.X_Reg = (value);
	cpu.PC++;
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE // meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_Y)
	{
		cpu.PC++;
	}
	set_zero(cpu.X_Reg, cpu);
	set_negative(cpu.X_Reg, cpu);
}
void LDY(uint8_t current_instruction, CPU &cpu)
{
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0xA0] = AddressMode::IMMEDIATE;
	address_Mode_map[0xA4] = AddressMode::ZERO_PAGE;
	address_Mode_map[0xB4] = AddressMode::ZERO_PAGE_X;
	address_Mode_map[0xAC] = AddressMode::ABSOLUTE;
	address_Mode_map[0xBC] = AddressMode::ABSOLUTE_X;
	uint8_t value = cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
												   cpu));
	cpu.Y_Reg = (value);
	cpu.PC++;
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE // meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X)
	{
		cpu.PC++;
	}
	set_zero(cpu.Y_Reg, cpu);
	set_negative(cpu.Y_Reg, cpu);
}
void PLP(uint8_t current_instruction, CPU &cpu)
{
	// TODO: rotate left
	cpu.status = cpu.bus.read_8bit(cpu.stack_pointer);
	cpu.stack_pointer++;
}
void PHP(uint8_t current_instruction, CPU &cpu)
{
	// TODO: rotate left
	cpu.stack_pointer--;
	cpu.bus.write_8bit(cpu.stack_pointer, cpu.status);
}
void PHA(uint8_t current_instruction, CPU &cpu)
{
	// push accumalortor on stack
	cpu.stack_pointer--;
	cpu.bus.write_8bit(cpu.stack_pointer, cpu.A_Reg);
}
void PLA(uint8_t current_instruction, CPU &cpu)
{
	// pull accumulator
	cpu.A_Reg = cpu.bus.read_8bit(cpu.stack_pointer);
	cpu.stack_pointer++;

	set_zero(cpu.A_Reg, cpu);
	set_negative(cpu.A_Reg, cpu);
}
void STA(uint8_t current_instruction, CPU &cpu)
{
	// TODO store accumulator in mem
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0x85] = AddressMode::ZERO_PAGE;
	address_Mode_map[0x95] = AddressMode::ZERO_PAGE_X;
	address_Mode_map[0x8D] = AddressMode::ABSOLUTE;
	address_Mode_map[0x9D] = AddressMode::ABSOLUTE_X;
	address_Mode_map[0x99] = AddressMode::ABSOLUTE_Y;
	address_Mode_map[0x81] = AddressMode::INDIRECT_X;
	address_Mode_map[0x91] = AddressMode::INDIRECT_Y;

	uint16_t v = address_Mode(address_Mode_map[current_instruction],
							  cpu);
	// printf("%x \n", v);
	// exit(EXIT_FAILURE);
	cpu.bus.write_8bit(v, cpu.A_Reg);
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE		// meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X // meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_Y)
		cpu.PC++;
	cpu.PC++;
}
void STX(uint8_t current_instruction, CPU &cpu)
{
	// TODO store x in mem
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0x86] = AddressMode::ZERO_PAGE; // nice
	address_Mode_map[0x96] = AddressMode::ZERO_PAGE_Y;
	address_Mode_map[0x8E] = AddressMode::ABSOLUTE;
	uint16_t v = address_Mode(address_Mode_map[current_instruction],
							  cpu);
	cpu.bus.write_8bit(v, cpu.X_Reg);
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE)
		cpu.PC++;
	cpu.PC++;
}
void STY(uint8_t current_instruction, CPU &cpu)
{
	// store y in mem
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0x84] = AddressMode::ZERO_PAGE;
	address_Mode_map[0x94] = AddressMode::ZERO_PAGE_X;
	address_Mode_map[0x8C] = AddressMode::ABSOLUTE; // nice
	uint16_t v = address_Mode(address_Mode_map[current_instruction],
							  cpu);
	cpu.bus.write_8bit(v, cpu.Y_Reg);
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE)
		cpu.PC++;
	cpu.PC++;
}
void TAY(uint8_t current_instruction, CPU &cpu)
{
	// Y = A
	cpu.Y_Reg = cpu.A_Reg;
	set_zero(cpu.Y_Reg, cpu);
	set_negative(cpu.Y_Reg, cpu);
}
void TYA(uint8_t current_instruction, CPU &cpu)
{
	// A = Y
	cpu.A_Reg = cpu.Y_Reg;
	set_zero(cpu.A_Reg, cpu);
	set_negative(cpu.A_Reg, cpu);
}
void TAX(uint8_t current_instruction, CPU &cpu)
{
	// X = A
	cpu.X_Reg = cpu.A_Reg;
	set_zero(cpu.X_Reg, cpu);
	set_negative(cpu.X_Reg, cpu);
}
void TXA(uint8_t current_instruction, CPU &cpu)
{
	// A = X
	cpu.A_Reg = cpu.X_Reg;
	set_zero(cpu.A_Reg, cpu);
	set_negative(cpu.A_Reg, cpu);
}
void TSX(uint8_t current_instruction, CPU &cpu)
{
	// x = S
	cpu.X_Reg = cpu.stack_pointer;
	set_zero(cpu.X_Reg, cpu);
	set_negative(cpu.X_Reg, cpu);
}
void TXS(uint8_t current_instruction, CPU &cpu)
{
	// S = X
	cpu.stack_pointer = cpu.X_Reg;
}
#pragma endregion region data transfer instructions

// ALU instructions, +,-,&, >>,<<
#pragma region ALU instructions
void ADC(uint8_t current_instruction, CPU &cpu)
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
	uint8_t value = cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
												   cpu));
	// cout << "est" << endl;
	if (check_decimal(cpu) != 0)
		cpu.A_Reg = decimal_add(cpu.A_Reg, value, cpu, carry);
	else
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
void SBC(uint8_t current_instruction, CPU &cpu)
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
	uint8_t value = cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
												   cpu));
	if (check_decimal(cpu) != 0)
		cpu.A_Reg = decimal_sub(cpu.A_Reg, value, cpu, carry);
	else
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
	cpu.A_Reg -= carry;
	cpu.PC++;
}
void BIT(uint8_t current_instruction, CPU &cpu)
{
	// TODO: bit test
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0x24] = AddressMode::ZERO_PAGE; // meow :3
	address_Mode_map[0x2C] = AddressMode::ABSOLUTE;	 // meow :3
	uint8_t value = cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
												   cpu));
	uint8_t result = value & cpu.A_Reg;
	set_zero(result, cpu);
	set_overflow((value & 0b00100000) != 0, cpu);
	set_negative(value, cpu);
	cpu.PC++;
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE)
		cpu.PC++;
}
void AND(uint8_t current_instruction, CPU &cpu)
{
	// TODO: and
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0x29] = AddressMode::IMMEDIATE; // meow :3

	address_Mode_map[0x25] = AddressMode::ZERO_PAGE;   // meow :3
	address_Mode_map[0x35] = AddressMode::ZERO_PAGE_X; // meow :3

	address_Mode_map[0x2D] = AddressMode::ABSOLUTE;	  // meow :3
	address_Mode_map[0x3D] = AddressMode::ABSOLUTE_X; // meow :3
	address_Mode_map[0x39] = AddressMode::ABSOLUTE_Y; // meow :3

	address_Mode_map[0x21] = AddressMode::INDIRECT_X; // meow :3
	address_Mode_map[0x31] = AddressMode::INDIRECT_Y; // meow :3

	uint8_t value = cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
												   cpu));
	cpu.A_Reg = cpu.A_Reg & value;
	set_negative(cpu.A_Reg, cpu);
	set_zero(cpu.A_Reg, cpu);
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE		// meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X // meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_Y)
	{
		cpu.PC++;
	}
	cpu.PC++;
}
void ORA(uint8_t current_instruction, CPU &cpu)
{
	// TODO: or
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0x09] = AddressMode::IMMEDIATE; // meow :3

	address_Mode_map[0x05] = AddressMode::ZERO_PAGE;   // meow :3
	address_Mode_map[0x15] = AddressMode::ZERO_PAGE_X; // meow :3

	address_Mode_map[0x0D] = AddressMode::ABSOLUTE;	  // meow :3
	address_Mode_map[0x1D] = AddressMode::ABSOLUTE_X; // meow :3
	address_Mode_map[0x19] = AddressMode::ABSOLUTE_Y; // meow :3

	address_Mode_map[0x01] = AddressMode::INDIRECT_X; // meow :3
	address_Mode_map[0x11] = AddressMode::INDIRECT_Y; // meow :3

	uint8_t value = cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
												   cpu));
	cpu.A_Reg = cpu.A_Reg | value;
	set_negative(cpu.A_Reg, cpu);
	set_zero(cpu.A_Reg, cpu);
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE		// meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X // meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_Y)
	{
		cpu.PC++;
	}
	cpu.PC++;
}
void ROR(uint8_t current_instruction, CPU &cpu)
{
	// TODO: rotate right

	set_carry((cpu.A_Reg & 1) != 0, cpu);
	if (current_instruction == 0x6A)
	{
		// accumalatpr
		cpu.A_Reg = rightRotate(cpu.A_Reg, 1);
	}
	else
	{
		map<uint8_t, AddressMode> address_Mode_map;

		address_Mode_map[0x66] = AddressMode::ZERO_PAGE;   // meow :3
		address_Mode_map[0x76] = AddressMode::ZERO_PAGE_X; // meow :3

		address_Mode_map[0x6E] = AddressMode::ABSOLUTE;	  // meow :3
		address_Mode_map[0x7E] = AddressMode::ABSOLUTE_X; // meow :3

		uint16_t address = address_Mode(address_Mode_map[current_instruction], cpu);
		uint8_t value = cpu.bus.read_8bit(address);
		value = rightRotate(value, 1);
		cpu.bus.write_8bit(address, value);

		if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE // meow
			|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X)
			cpu.PC++;
		cpu.PC++;
	}
	set_negative(cpu.A_Reg, cpu);
	set_zero(cpu.A_Reg, cpu);
}
void ROL(uint8_t current_instruction, CPU &cpu)
{
	set_carry(((cpu.A_Reg >> 7) != 0), cpu);
	if (current_instruction == 0x2A)
	{
		// accumalatpr
		cpu.A_Reg = leftRotate(cpu.A_Reg, 1);
	}
	else
	{
		map<uint8_t, AddressMode> address_Mode_map;

		address_Mode_map[0x26] = AddressMode::ZERO_PAGE;   // meow :3
		address_Mode_map[0x36] = AddressMode::ZERO_PAGE_X; // meow :3

		address_Mode_map[0x2E] = AddressMode::ABSOLUTE;	  // meow :3
		address_Mode_map[0x3E] = AddressMode::ABSOLUTE_X; // meow :3
		uint16_t address = address_Mode(address_Mode_map[current_instruction], cpu);
		uint8_t value = cpu.bus.read_8bit(address);
		value = leftRotate(value, 1);
		cpu.bus.write_8bit(address, value);
		if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE // meow
			|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X)
			cpu.PC++;
		cpu.PC++;
	}
	set_negative(cpu.A_Reg, cpu);
	set_zero(cpu.A_Reg, cpu);
	// TODO: rotate left
}
void ASL(uint8_t current_instruction, CPU &cpu)
{
	// TODO a >> m
	set_carry(((cpu.A_Reg >> 7) != 0), cpu);

	if (current_instruction == 0x0A)
	{
		cpu.A_Reg = cpu.A_Reg << 1;
	}
	else
	{
		map<uint8_t, AddressMode> address_Mode_map;

		address_Mode_map[0x06] = AddressMode::ZERO_PAGE;   // meow :3
		address_Mode_map[0x16] = AddressMode::ZERO_PAGE_X; // meow :3

		address_Mode_map[0x0E] = AddressMode::ABSOLUTE;	  // meow :3
		address_Mode_map[0x1E] = AddressMode::ABSOLUTE_X; // meow :3
		uint16_t address = address_Mode(address_Mode_map[current_instruction], cpu);
		uint8_t value = cpu.bus.read_8bit(address);

		// cpu.A_Reg = cpu.A_Reg << value;
		value <<= 1;
		cpu.bus.write_8bit(address, value);
		if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE // meow
			|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X)
			cpu.PC++;
		cpu.PC++;
	}
	set_negative(cpu.A_Reg, cpu);
	set_zero(cpu.A_Reg, cpu);
}
void LSR(uint8_t current_instruction, CPU &cpu)
{
	set_carry((cpu.A_Reg & 1) != 0, cpu);
	if (current_instruction == 0x4A)
	{
		cpu.A_Reg = cpu.A_Reg >> 1;
	}
	else
	{
		map<uint8_t, AddressMode> address_Mode_map;

		address_Mode_map[0x46] = AddressMode::ZERO_PAGE;   // meow :3
		address_Mode_map[0x56] = AddressMode::ZERO_PAGE_X; // meow :3

		address_Mode_map[0x4E] = AddressMode::ABSOLUTE;	  // meow :3
		address_Mode_map[0x5E] = AddressMode::ABSOLUTE_X; // meow :3
		uint16_t address = address_Mode(address_Mode_map[current_instruction], cpu);
		uint8_t value = cpu.bus.read_8bit(address);
		value >>= 1;
		cpu.bus.write_8bit(address, value);
		if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE // meow
			|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X)
			cpu.PC++;
		cpu.PC++;
	}
	set_negative(cpu.A_Reg, cpu);
	set_zero(cpu.A_Reg, cpu);

	// TODO a << m
}
void EOR(uint8_t current_instruction, CPU &cpu)
{
	// TODO xor
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0x49] = AddressMode::IMMEDIATE; // meow :3

	address_Mode_map[0x45] = AddressMode::ZERO_PAGE;   // meow :3
	address_Mode_map[0x55] = AddressMode::ZERO_PAGE_X; // meow :3

	address_Mode_map[0x4D] = AddressMode::ABSOLUTE;	  // meow :3
	address_Mode_map[0x5D] = AddressMode::ABSOLUTE_X; // meow :3
	address_Mode_map[0x59] = AddressMode::ABSOLUTE_Y; // meow :3

	address_Mode_map[0x41] = AddressMode::INDIRECT_X; // meow :3
	address_Mode_map[0x51] = AddressMode::INDIRECT_Y; // meow :3
	uint8_t value = cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
												   cpu));
	cpu.A_Reg = cpu.A_Reg ^ value;

	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE		// meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X // meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_Y)
	{
		cpu.PC++;
	}
	set_negative(cpu.A_Reg, cpu);
	set_zero(cpu.A_Reg, cpu);
	cpu.PC++;
}
void DEX(uint8_t current_instruction, CPU &cpu)
{
	// TODO:x--
	cpu.X_Reg--;
	set_negative(cpu.X_Reg, cpu);
	set_zero(cpu.X_Reg, cpu);
}
void DEY(uint8_t current_instruction, CPU &cpu)
{
	// TODO y--
	cpu.Y_Reg--;
	set_negative(cpu.Y_Reg, cpu);
	set_zero(cpu.Y_Reg, cpu);
}
void DEC(uint8_t current_instruction, CPU &cpu)
{
	// TODO: m--
	map<uint8_t, AddressMode> address_Mode_map;

	address_Mode_map[0xC6] = AddressMode::ZERO_PAGE;   // meow :3
	address_Mode_map[0xD6] = AddressMode::ZERO_PAGE_X; // meow :3

	address_Mode_map[0xCE] = AddressMode::ABSOLUTE;	  // meow :3
	address_Mode_map[0xDE] = AddressMode::ABSOLUTE_X; // meow :3

	uint16_t v = address_Mode(address_Mode_map[current_instruction],
							  cpu);
	uint8_t vs = cpu.bus.read_8bit(v);
	vs--;
	set_zero(vs, cpu);
	set_negative(vs, cpu);
	cpu.bus.write_8bit(v, vs);
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE // meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X)
		cpu.PC++;
	cpu.PC++;
}

void INC(uint8_t current_instruction, CPU &cpu)
{
	// TODO: m--
	map<uint8_t, AddressMode> address_Mode_map;

	address_Mode_map[0xE6] = AddressMode::ZERO_PAGE;   // meow :3
	address_Mode_map[0xF6] = AddressMode::ZERO_PAGE_X; // meow :3

	address_Mode_map[0xEE] = AddressMode::ABSOLUTE;	  // meow :3
	address_Mode_map[0xFE] = AddressMode::ABSOLUTE_X; // meow :3
	uint16_t v = address_Mode(address_Mode_map[current_instruction],
							  cpu);
	uint8_t vs = cpu.bus.read_8bit(v);
	vs++;
	cpu.bus.write_8bit(v, vs);
	set_zero(vs, cpu);
	set_negative(vs, cpu);
	// printf("%d \n", vs);
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE // meow
		|| address_Mode_map[current_instruction] == AddressMode::ABSOLUTE_X)
		cpu.PC++;
	cpu.PC++;
}
void INX(uint8_t current_instruction, CPU &cpu)
{
	// TODO: m--
	cpu.X_Reg++;
	set_negative(cpu.X_Reg, cpu);
	set_zero(cpu.X_Reg, cpu);
}
void INY(uint8_t current_instruction, CPU &cpu)
{
	// TODO: m--
	cpu.Y_Reg++;
	set_negative(cpu.Y_Reg, cpu);
	set_zero(cpu.Y_Reg, cpu);
}

#pragma endregion ALU instructions

// status register  instructions
#pragma region setFlags
void CLV(uint8_t current_instruction, CPU &cpu)
{
	// set_overflow(0, cpu);
	set_overflow(0, cpu);
}

void CLC(uint8_t current_instruction, CPU &cpu)
{
	set_carry(0, cpu);
}
void SEC(uint8_t current_instruction, CPU &cpu)
{
	set_carry(1, cpu);
}
void SEI(uint8_t current_instruction, CPU &cpu)
{
	set_interrupt_disabled(1, cpu);
}
void CLI(uint8_t current_instruction, CPU &cpu)
{
	set_interrupt_disabled(0, cpu);
}

void SED(uint8_t current_instruction, CPU &cpu)
{
	set_decimal_mode(1, cpu);
}
void CLD(uint8_t current_instruction, CPU &cpu)
{
	set_decimal_mode(0, cpu);
}
void RTI(uint8_t current_instruction, CPU &cpu)
{
	// TODO:return from interrupt
	cpu.PC = cpu.bus.read_16bit(cpu.stack_pointer);
	cpu.stack_pointer += 2;
	cpu.status = cpu.bus.read_8bit(cpu.stack_pointer);
	set_brk(cpu, 0);
}
#pragma endregion setFlags

// conditional branching, Stack functions and Jumping  instructions as well as CMP
#pragma region Jmp
void JMP(uint8_t current_instruction, CPU &cpu)
{

	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0x4C] = AddressMode::ABSOLUTE;
	address_Mode_map[0x6C] = AddressMode::INDIRECT;
	uint16_t new_PC = address_Mode(address_Mode_map[current_instruction],
								   cpu);
	cpu.PC = (new_PC);

	// printf(" PC: %x \n", cpu.PC);
}
void BEQ(uint8_t current_instruction, CPU &cpu)
{
	map<uint8_t, AddressMode> address_Mode_map;

	address_Mode_map[0xF0] = AddressMode::IMMEDIATE;
	int8_t new_PC = (int8_t)cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
														   cpu));
	if (check_zero(cpu) == 0)
	{
		cpu.PC++;
		return;
	}
	cpu.PC += (int8_t)new_PC;
	cpu.PC++; // cpu.PC = (new_PC + 0x8000);
}
void BNE(uint8_t current_instruction, CPU &cpu)
{
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0xD0] = AddressMode::IMMEDIATE;
	int8_t new_PC = cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
												   cpu));
	if (check_zero(cpu) != 0)
	{
		cpu.PC++;
		// cout << "0" << endl;
		// printf("0 %d \n", check_zero(cpu));
		return;
	}

	// cpu.PC++;
	cpu.PC += (int8_t)new_PC;
	cpu.PC++;
	// cpu.PC = (new_PC + 0x8000);
}
void BCC(uint8_t current_instruction, CPU &cpu)
{
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0x90] = AddressMode::IMMEDIATE;
	int8_t new_PC = (int16_t)cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
															cpu));
	if (check_carry(cpu) != 0)
	{
		cpu.PC++;
		return;
	}

	// cpu.PC = (new_PC + 0x8000);
	cpu.PC += (int8_t)new_PC;
	cpu.PC++;
}
void BCS(uint8_t current_instruction, CPU &cpu)
{
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0xB0] = AddressMode::IMMEDIATE;
	int8_t new_PC = (int8_t)cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
														   cpu));
	if (check_carry(cpu) == 0)
	{
		cpu.PC++;
		return;
	}

	// cpu.PC = (new_PC + 0x8000);
	cpu.PC += (int8_t)new_PC;
	cpu.PC++;
}
void BPL(uint8_t current_instruction, CPU &cpu)
{
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0x10] = AddressMode::IMMEDIATE;
	int8_t new_PC = (int8_t)cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
														   cpu));
	if (check_negative(cpu) != 0)
	{
		cpu.PC++;
		return;
	}

	// cpu.PC = (new_PC + 0x8000);
	cpu.PC += (int8_t)new_PC;
	cpu.PC++;
}
void BMI(uint8_t current_instruction, CPU &cpu)
{
	// TODO: Branch if negatice
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0x30] = AddressMode::IMMEDIATE;
	int8_t new_PC = (int8_t)cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
														   cpu));
	if (check_negative(cpu) == 0)
	{
		cpu.PC++;
		return;
	}

	// cpu.PC = (new_PC + 0x8000);
	cpu.PC += (int8_t)new_PC;
	cpu.PC++;
}
void BVC(uint8_t current_instruction, CPU &cpu)
{
	// TODO: Branch if overflow clear
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0x50] = AddressMode::IMMEDIATE;
	int8_t new_PC = (int8_t)cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
														   cpu));
	if (check_overflow(cpu) != 0)
	{
		cpu.PC++;
		return;
	}

	// cpu.PC = (new_PC + 0x8000);
	cpu.PC += (int8_t)new_PC;
	cpu.PC++;
}
void BVS(uint8_t current_instruction, CPU&cpu)
{
	// TODO: Branch if overflow set
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0x70] = AddressMode::IMMEDIATE;
	int8_t new_PC = (int8_t)cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
														   cpu));
	if (check_overflow(cpu) == 0)
	{
		cpu.PC++;
		return;
	}

	// cpu.PC = (new_PC + 0x8000);
	cpu.PC += (int8_t)new_PC;
	cpu.PC++;
}
void JSR(uint8_t current_instruction, CPU &cpu)
{
	// TODO: functions
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0x20] = AddressMode::ABSOLUTE;
	uint16_t new_PC = (uint16_t)(address_Mode(address_Mode_map[current_instruction],
											  cpu)); 
	cpu.PC += 2;
	cpu.stack_pointer -= 2;
	cpu.bus.write_16bit(cpu.stack_pointer, cpu.PC);
	// cpu.PC = (new_PC + 0x8000);
	cpu.PC = new_PC;
}
void RTS(uint8_t current_instruction, CPU &cpu)
{

	// cpu.stack_pointer += 2;
	cpu.PC = cpu.bus.read_16bit(cpu.stack_pointer);
	// printf("sp: %x \n", cpu.stack_pointer);

	// printf("pc: %x \n", cpu.bus.read_16bit(cpu.stack_pointer));
	cpu.stack_pointer += 2;
	// cpu.PC++;

	// cpu.stack_pointer += 2;
}
void CMP(uint8_t current_instruction, CPU &cpu)
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
	uint8_t value = cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
												   cpu));
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
void CPY(uint8_t current_instruction, CPU &cpu)
{
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0xC0] = AddressMode::IMMEDIATE;
	address_Mode_map[0xC4] = AddressMode::ZERO_PAGE;
	address_Mode_map[0xCC] = AddressMode::ABSOLUTE;
	uint8_t value = cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
												   cpu));
	uint8_t carry = 0;
	uint8_t v = sub(cpu.Y_Reg, value, cpu, carry);
	// printf("X_Reg: %d \n", v);
	// printf("carry: %d \n", carry);
	set_carry(carry, cpu);
	set_zero(v, cpu);
	set_negative(v, cpu);
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE)
		cpu.PC++;
	cpu.PC++;
}
void CPX(uint8_t current_instruction, CPU &cpu)
{
	map<uint8_t, AddressMode> address_Mode_map;
	address_Mode_map[0xE0] = AddressMode::IMMEDIATE;
	address_Mode_map[0xE4] = AddressMode::ZERO_PAGE;
	address_Mode_map[0xEC] = AddressMode::ABSOLUTE;

	uint8_t value = cpu.bus.read_8bit(address_Mode(address_Mode_map[current_instruction],
												   cpu));
	uint8_t carry = 0;

	uint8_t v = sub(cpu.X_Reg, value, cpu, carry);
	// printf("value: %d result %d \n", value, v);

	// printf("carry: %d \n", carry);
	set_carry(carry, cpu);
	set_zero(v, cpu);
	set_negative(v, cpu);
	if (address_Mode_map[current_instruction] == AddressMode::ABSOLUTE)
		cpu.PC++;
	cpu.PC++;
}

#pragma endregion Jmp