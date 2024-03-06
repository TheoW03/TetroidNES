#include <iostream>
#include <vector>
#include "LoadRom.h"
using namespace std;

#define RAM_START 0x0000
#define RAM_MIRRORS_END 0x0FFF

#define PPU_REGISTERS 0x2000
#define PPU_REGISTERS_END 0x3FFF

static volatile uint8_t memory[0xFFFF];
static uint8_t v_memory[0x800];
static uint16_t reset_vector;

// struct Bus
// {
// 	uint8_t v_memory[0x800];
// 	virtual uint8_t read_8bit(uint16_t address);
// 	virtual void write_8bit(uint16_t address, uint8_t value);
// 	virtual uint16_t read_16bit(uint16_t address, uint8_t value);
// 	virtual void write_16bit(uint16_t address, uint16_t value);
// };
// void Bus::read_16bit(uint16_t)
// {
// 	cout << "hello world" << endl;
// }
uint8_t read_8bit(uint16_t address)
{
	return memory[address];
}

uint16_t read_16bit(uint16_t address)
{
	uint16_t value = (uint16_t)(memory[address + 1] << 8) | memory[address];

	return value;
}
void write_8bit(uint16_t address, uint8_t value)
{
	memory[address] = value;
}
void write_16bit(uint16_t address, uint16_t value)
{
	uint8_t msb = (uint8_t)(value >> 8);
	uint8_t lsb = (uint8_t)(value & 0xFF);
	memory[address] = lsb;
	memory[address + 1] = msb;
}

void load_instructions(vector<uint8_t> instructions)
{
	cout << "loaded instructions" << endl;
	cout << "===============" << endl;
	for (size_t i = 0; i < instructions.size(); i++)
	{
		printf("PC: %x  instruction: %x \n", (uint16_t)(i + 0x600), instructions[i]);
		write_8bit((i + 0x600), instructions[i]);
	}
	cout << "===============" << endl;
}

uint8_t NES_mmap_read(uint16_t address)
{
	if (address < RAM_MIRRORS_END)
	{
		uint16_t mirroraddr = address & (1 << 11);
		return v_memory[mirroraddr];
	}
	else if (address >= PPU_REGISTERS && address <= PPU_REGISTERS_END)
	{
		return 0;
	}
}
void NES_mmap_write(uint16_t address, uint8_t value)
{
	if (address < RAM_MIRRORS_END)
	{
		uint16_t mirroraddr = address & (1 << 11);
		v_memory[mirroraddr] = value;
	}
	else if (address >= PPU_REGISTERS && address <= PPU_REGISTERS_END)
	{
	}
}

uint16_t NES_mmap_read16(uint16_t address)
{
	if (address < RAM_MIRRORS_END)
	{
		uint16_t mirroraddr = address & (1 << 11);
		return (uint16_t)(v_memory[mirroraddr + 1] << 8) | v_memory[mirroraddr];
	}
	else if (address >= PPU_REGISTERS && address <= PPU_REGISTERS_END)
	{
		return 0;
	}
}
void NES_mmap_write16(uint16_t address, uint16_t value)
{
	if (address < RAM_MIRRORS_END)
	{
		uint16_t mirroraddr = address & (1 << 11);
		uint8_t msb = (uint8_t)(value >> 8);
		uint8_t lsb = (uint8_t)(value & 0xFF);
		v_memory[mirroraddr] = lsb;
		v_memory[mirroraddr + 1] = msb;
	}
	else if (address >= PPU_REGISTERS && address <= PPU_REGISTERS_END)
	{
		// return 0;
	}
}
// void load_rom(Rom rom)
// {
// 	for (int i = rom.PRG_ROM_START; i < rom.PRG_ROM_SIZE; i++)
// 	{
// 		PRGrom.push_back(rom.instructions[i]);
// 	}
// 	for (int i = rom.CHR_ROM_START; i < rom.CHR_ROM_SIZE; i++)
// 	{
// 		CHRrom.push_back(rom.instructions[i]);
// 	}
// }