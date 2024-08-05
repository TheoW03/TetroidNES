#include <iostream>

#include "BitOperations.h"
#include "Memory.h"
#include "Computer.h"
#include "AddressMode.h"
#include "StatusRegister.h"
#include "Bus.h"

uint16_t address_mode(AddressMode address, CPU &cpu)
{
	switch (address)
	{
	case AddressMode::IMMEDIATE:
	{
		return immediate_address_mode(cpu);
	}
	case AddressMode::ZERO_PAGE:
	{
		return zero_page_address_mode(cpu);
	}
	case AddressMode::RELATIVE:
	{
		return relative_addressmode(cpu);
	}
	case AddressMode::ZERO_PAGE_X:
	{
		return zero_page_address_mode_X(cpu);
	}
	case AddressMode::ZERO_PAGE_Y:
	{
		return zero_page_address_mode_Y(cpu);
	}
	case AddressMode::ABSOLUTE:
	{
		return absolute(cpu);
	}
	case AddressMode::ABSOLUTE_X:
	{
		return absolute_page_address_mode_X(cpu);
	}
	case AddressMode::ABSOLUTE_Y:
	{
		return absolute_page_address_mode_Y(cpu);
	}
	case AddressMode::INDIRECT:
	{
		return indirect_address_mode(cpu);
	}
	case AddressMode::INDIRECT_X:
	{
		return indirect_address_mode_X(cpu);
	}
	case AddressMode::INDIRECT_Y:
	{
		return indirect_address_Mode_Y(cpu);
	}
	default:
	{
		std::cout << "Instruction not supported." << std::endl;
		return 0;
	}
	}
}
uint8_t get_value(AddressMode address, CPU &cpu)
{
	if (address == AddressMode::IMMEDIATE || address == AddressMode::RELATIVE)
	{
		return cpu.bus.fetch_next();
	}
	return cpu.bus.read_8bit(address_mode(address, cpu));
}
// data transfer instructions, includes MOV...Stack pointer what not
#pragma region data transfer instructions
void LDA(AddressMode addressType, CPU &cpu)
{
	uint8_t value = get_value(addressType, cpu);
	cpu.A_Reg = value;
	set_zero(cpu.A_Reg, cpu);
	set_negative(cpu.A_Reg, cpu);
}

void LDX(AddressMode addressType, CPU &cpu)
{
	uint8_t value = get_value(addressType, cpu);
	cpu.X_Reg = value;
	set_zero(cpu.X_Reg, cpu);
	set_negative(cpu.X_Reg, cpu);
}

void LDY(AddressMode addressType, CPU &cpu)
{
	uint8_t value = get_value(addressType, cpu);
	cpu.Y_Reg = (value);
	set_zero(cpu.Y_Reg, cpu);
	set_negative(cpu.Y_Reg, cpu);
}

void PLP(AddressMode addressType, CPU &cpu)
{
	// TODO: rotate left
	cpu.status = cpu.bus.pop_stack8();
	// cpu.stack_pointer++;
}

void PHP(AddressMode addressType, CPU &cpu)
{
	// TODO: rotate left
	// cpu.stack_pointer--;
	// cpu.bus.write_8bit(cpu.stack_pointer, cpu.status);

	cpu.bus.push_stack8(cpu.status);
}

void PHA(AddressMode addressType, CPU &cpu)
{
	// push accumulator on stack
	// cpu.stack_pointer--;
	// cpu.bus.write_8bit(cpu.stack_pointer, cpu.A_Reg);
	cpu.bus.push_stack8(cpu.A_Reg);
}

void PLA(AddressMode addressType, CPU &cpu)
{
	// pull accumulator
	cpu.A_Reg = cpu.bus.pop_stack8();
	// cpu.stack_pointer++;

	set_zero(cpu.A_Reg, cpu);
	set_negative(cpu.A_Reg, cpu);
}

void STA(AddressMode addressType, CPU &cpu)
{
	// TODO store accumulator in mem
	uint16_t v = address_mode(addressType, cpu);
	cpu.bus.write_8bit(v, cpu.A_Reg);
}

void STX(AddressMode addressType, CPU &cpu)
{
	uint16_t v = address_mode(addressType, cpu);
	cpu.bus.write_8bit(v, cpu.X_Reg);
}

void STY(AddressMode addressType, CPU &cpu)
{
	// store y in mem
	uint16_t v = address_mode(addressType, cpu);
	cpu.bus.write_8bit(v, cpu.Y_Reg);
}

void TAY(AddressMode addressType, CPU &cpu)
{
	// Y = A
	cpu.Y_Reg = cpu.A_Reg;
	set_zero(cpu.Y_Reg, cpu);
	set_negative(cpu.Y_Reg, cpu);
}

void TYA(AddressMode addressType, CPU &cpu)
{
	// A = Y
	cpu.A_Reg = cpu.Y_Reg;
	set_zero(cpu.A_Reg, cpu);
	set_negative(cpu.A_Reg, cpu);
}

void TAX(AddressMode addressType, CPU &cpu)
{
	// X = A
	cpu.X_Reg = cpu.A_Reg;
	set_zero(cpu.X_Reg, cpu);
	set_negative(cpu.X_Reg, cpu);
}

void TXA(AddressMode addressType, CPU &cpu)
{
	// A = X
	cpu.A_Reg = cpu.X_Reg;
	set_zero(cpu.A_Reg, cpu);
	set_negative(cpu.A_Reg, cpu);
}

void TSX(AddressMode addressType, CPU &cpu)
{
	// X = S
	// cpu.X_Reg = cpu.stack_pointer;
	cpu.X_Reg = cpu.bus.get_stack_pointer();
	set_zero(cpu.X_Reg, cpu);
	set_negative(cpu.X_Reg, cpu);
}

void TXS(AddressMode addressType, CPU &cpu)
{
	// S = X
	// cpu.stack_pointer = cpu.X_Reg;
	cpu.bus.set_stack_pointer(cpu.X_Reg);
}

#pragma endregion region data transfer instructions

// ALU instructions, +,-,&, >>,<<
#pragma region ALU instructions
void ADC(AddressMode addressType, CPU &cpu)
{

	uint8_t carry = 0;
	uint8_t value = get_value(addressType, cpu);
	if (check_decimal(cpu) != 0)
	{
		cpu.A_Reg = decimal_add(cpu.A_Reg, value, cpu, carry);
	}
	else
	{
		cpu.A_Reg = add(cpu.A_Reg, value, cpu, carry);
	}

	set_carry(carry, cpu);
	set_zero(cpu.A_Reg, cpu);
	set_negative(cpu.A_Reg, cpu);
	if (check_carry(cpu) != 0)
	{
		cpu.A_Reg++;
	}
}

void SBC(AddressMode addressType, CPU &cpu)
{

	uint8_t carry = 0;
	uint8_t value = get_value(addressType, cpu);
	if (check_decimal(cpu) != 0)
	{
		cpu.A_Reg = decimal_sub(cpu.A_Reg, value, cpu, carry);
	}
	else
	{
		cpu.A_Reg = sub(cpu.A_Reg, value, cpu, carry);
	}

	set_carry(carry, cpu);
	set_zero(cpu.A_Reg, cpu);
	set_negative(cpu.A_Reg, cpu);
	cpu.A_Reg -= carry;
}

void BIT(AddressMode addressType, CPU &cpu)
{
	// TODO: bit test
	uint8_t value = get_value(addressType, cpu);
	uint8_t result = value & cpu.A_Reg;
	set_zero(result, cpu);
	set_overflow((value & 0b00100000) != 0, cpu);
	set_negative(value, cpu);
}

void AND(AddressMode addressType, CPU &cpu)
{
	// TODO: and
	uint8_t value = get_value(addressType, cpu);
	cpu.A_Reg = cpu.A_Reg & value;
	set_negative(cpu.A_Reg, cpu);
	set_zero(cpu.A_Reg, cpu);
}

void ORA(AddressMode addressType, CPU &cpu)
{
	// TODO: or

	uint8_t value = get_value(addressType, cpu);
	cpu.A_Reg = cpu.A_Reg | value;
	set_negative(cpu.A_Reg, cpu);
	set_zero(cpu.A_Reg, cpu);
}

void ROR(AddressMode addressType, CPU &cpu)
{
	// TODO: rotate right

	set_carry((cpu.A_Reg & 1) != 0, cpu);
	if (addressType == AddressMode::ACCUMULATOR)
	{
		cpu.A_Reg = right_rotate(cpu.A_Reg, 1);
		set_negative(cpu.A_Reg, cpu);
		set_zero(cpu.A_Reg, cpu);
	}
	else
	{

		uint16_t address = address_mode(addressType, cpu);
		uint8_t value = cpu.bus.read_8bit(address);
		value = right_rotate(value, 1);
		cpu.bus.write_8bit(address, value);
		set_negative(value, cpu);
		set_zero(value, cpu);
	}
}

void ROL(AddressMode addressType, CPU &cpu)
{
	set_carry(((cpu.A_Reg >> 7) != 0), cpu);
	if (addressType == AddressMode::ACCUMULATOR)
	{
		// accumulator
		cpu.A_Reg = left_rotate(cpu.A_Reg, 1);
		set_negative(cpu.A_Reg, cpu);
		set_zero(cpu.A_Reg, cpu);
	}
	else
	{

		uint16_t address = address_mode(addressType, cpu);
		uint8_t value = cpu.bus.read_8bit(address);
		value = left_rotate(value, 1);
		cpu.bus.write_8bit(address, value);
		set_negative(value, cpu);
		set_zero(value, cpu);
	}

	// TODO: rotate left
}

void ASL(AddressMode addressType, CPU &cpu)
{
	// TODO a >> m
	set_carry(((cpu.A_Reg >> 7) != 0), cpu);

	if (addressType == AddressMode::ACCUMULATOR)
	{
		cpu.A_Reg = cpu.A_Reg << 1;
		set_negative(cpu.A_Reg, cpu);
		set_zero(cpu.A_Reg, cpu);
	}
	else
	{

		uint16_t address = address_mode(addressType, cpu);
		uint8_t value = cpu.bus.read_8bit(address);

		// cpu.A_Reg = cpu.A_Reg << value;
		value <<= 1;
		cpu.bus.write_8bit(address, value);
		set_negative(value, cpu);
		set_zero(value, cpu);
	}
}

void LSR(AddressMode addressType, CPU &cpu)
{
	set_carry((cpu.A_Reg & 1) != 0, cpu);
	if (addressType == AddressMode::ACCUMULATOR)
	{
		cpu.A_Reg = cpu.A_Reg >> 1;
		set_negative(cpu.A_Reg, cpu);
		set_zero(cpu.A_Reg, cpu);
	}
	else
	{

		uint16_t address = address_mode(addressType, cpu);
		uint8_t value = cpu.bus.read_8bit(address);
		value >>= 1;
		cpu.bus.write_8bit(address, value);
		set_negative(value, cpu);
		set_zero(value, cpu);
	}

	// TODO a << m
}

void EOR(AddressMode addressType, CPU &cpu)
{
	// TODO xor
	uint8_t value = get_value(addressType, cpu);
	cpu.A_Reg = cpu.A_Reg ^ value;
}

void DEX(AddressMode addressType, CPU &cpu)
{
	// TODO:x--
	cpu.X_Reg--;
	set_negative(cpu.X_Reg, cpu);
	set_zero(cpu.X_Reg, cpu);
}

void DEY(AddressMode addressType, CPU &cpu)
{
	// TODO y--
	cpu.Y_Reg--;
	set_negative(cpu.Y_Reg, cpu);
	set_zero(cpu.Y_Reg, cpu);
}

void DEC(AddressMode addressType, CPU &cpu)
{
	// TODO: m--

	uint16_t v = address_mode(addressType, cpu);
	uint8_t vs = cpu.bus.read_8bit(v);
	vs--;
	set_zero(vs, cpu);
	set_negative(vs, cpu);
	cpu.bus.write_8bit(v, vs);
}

void INC(AddressMode addressType, CPU &cpu)
{
	// TODO: m--

	uint16_t v = address_mode(addressType, cpu);

	uint8_t vs = cpu.bus.read_8bit(v);
	vs++;
	cpu.bus.write_8bit(v, vs);
	set_zero(vs, cpu);
	set_negative(vs, cpu);
}

void INX(AddressMode addressType, CPU &cpu)
{
	// TODO: m--
	cpu.X_Reg++;
	set_negative(cpu.X_Reg, cpu);
	set_zero(cpu.X_Reg, cpu);
}

void INY(AddressMode addressType, CPU &cpu)
{
	// TODO: m--
	cpu.Y_Reg++;
	set_negative(cpu.Y_Reg, cpu);
	set_zero(cpu.Y_Reg, cpu);
}

#pragma endregion ALU instructions

// status register instructions
#pragma region setFlags
void CLV(AddressMode addressType, CPU &cpu)
{
	set_overflow(0, cpu);
}

void CLC(AddressMode addressType, CPU &cpu)
{
	set_carry(0, cpu);
}

void SEC(AddressMode addressType, CPU &cpu)
{
	set_carry(1, cpu);
}

void SEI(AddressMode addressType, CPU &cpu)
{
	set_interrupt_disabled(1, cpu);
}

void CLI(AddressMode addressType, CPU &cpu)
{
	set_interrupt_disabled(0, cpu);
}

void SED(AddressMode addressType, CPU &cpu)
{
	set_decimal_mode(1, cpu);
}

void CLD(AddressMode addressType, CPU &cpu)
{
	set_decimal_mode(0, cpu);
}

void RTI(AddressMode addressType, CPU &cpu)
{
	// TODO:return from interrupt
	cpu.bus.fill(cpu.bus.pop_stack16());
	cpu.status = cpu.bus.pop_stack8();
	set_brk(cpu, 0);
}
#pragma endregion setFlags

// conditional branching, Stack functions and Jumping instructions as well as CMP
#pragma region Jmp
void JMP(AddressMode addressType, CPU &cpu)
{

	uint16_t new_PC = address_mode(addressType, cpu);
	cpu.bus.fill(new_PC);
}

void BEQ(AddressMode addressType, CPU &cpu)
{
	int8_t new_PC = (int8_t)get_value(addressType, cpu);
	if (check_zero(cpu) == 0)
	{
		return;
	}
	cpu.bus.fill((uint16_t)((cpu.bus.get_PC() - 1) + new_PC));
}

void BNE(AddressMode addressType, CPU &cpu)
{

	int8_t new_PC = (int8_t)get_value(addressType, cpu);

	if (check_zero(cpu) != 0)
	{
		return;
	}
	uint16_t a = (uint16_t)((cpu.bus.get_PC() - 1) + (int8_t)new_PC);
	cpu.bus.fill(a);
}

void BCC(AddressMode addressType, CPU &cpu)
{

	int8_t new_PC = (int8_t)get_value(addressType, cpu);

	if (check_carry(cpu) != 0)
	{
		return;
	}
	cpu.bus.fill((uint16_t)((cpu.bus.get_PC() - 1) + new_PC));
	// cpu.PC = (new_PC + 0x8000);
}

void BCS(AddressMode addressType, CPU &cpu)
{

	int8_t new_PC = (int8_t)get_value(addressType, cpu);

	if (check_carry(cpu) == 0)
	{
		return;
	}

	cpu.bus.fill((uint16_t)((cpu.bus.get_PC() - 1) + new_PC));
}

void BPL(AddressMode addressType, CPU &cpu)
{
	int8_t new_PC = (int8_t)get_value(addressType, cpu);
	if (check_negative(cpu) != 0)
	{
		return;
	}

	cpu.bus.fill((uint16_t)((cpu.bus.get_PC() - 1) + new_PC));
}

void BMI(AddressMode addressType, CPU &cpu)
{
	// TODO: Branch if negative
	int8_t new_PC = (int8_t)get_value(addressType, cpu);

	if (check_negative(cpu) == 0)
	{
		return;
	}
	cpu.bus.fill((uint16_t)((cpu.bus.get_PC() - 1) + new_PC));
}

void BVC(AddressMode addressType, CPU &cpu)
{
	// TODO: Branch if overflow clear

	int8_t new_PC = (int8_t)get_value(addressType, cpu);

	if (check_overflow(cpu) != 0)
	{
		return;
	}

	// cpu.PC = (new_PC + 0x8000);
	cpu.bus.fill((uint16_t)((cpu.bus.get_PC() - 1) + new_PC));
}

void BVS(AddressMode addressType, CPU &cpu)
{
	// TODO: Branch if overflow set
	int8_t new_PC = (int8_t)get_value(addressType, cpu);
	if (check_overflow(cpu) == 0)
	{
		return;
	}

	cpu.bus.fill((uint16_t)((cpu.bus.get_PC() - 1) + new_PC));
}

void JSR(AddressMode addressType, CPU &cpu)
{
	// TODO: functions
	uint16_t new_PC = (uint16_t)(address_mode(addressType, cpu));
	cpu.bus.push_stack16(cpu.bus.get_PC()); // "potential bugs"
	cpu.bus.fill(new_PC);
}

void RTS(AddressMode addressType, CPU &cpu)
{
	cpu.bus.fill(cpu.bus.pop_stack16() - 1);
}

void CMP(AddressMode addressType, CPU &cpu)
{
	uint8_t carry = 0;
	uint8_t value = get_value(addressType, cpu);
	uint8_t v = sub(cpu.A_Reg, value, cpu, carry);
	set_carry(carry, cpu);
	set_zero(v, cpu);
	set_negative(v, cpu);
}

void CPY(AddressMode addressType, CPU &cpu)
{
	uint8_t value = get_value(addressType, cpu);
	uint8_t carry = 0;
	uint8_t v = sub(cpu.Y_Reg, value, cpu, carry);
	set_carry(carry, cpu);
	set_zero(v, cpu);
	set_negative(v, cpu);
}

void CPX(AddressMode addressType, CPU &cpu)
{

	uint8_t value = get_value(addressType, cpu);
	uint8_t carry = 0;

	uint8_t v = sub(cpu.X_Reg, value, cpu, carry);
	set_carry(carry, cpu);
	set_zero(v, cpu);
	set_negative(v, cpu);
}

#pragma endregion Jmp