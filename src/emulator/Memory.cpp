#include <iostream>
#include <vector>

using namespace std;

#define RAM_START 0x0000
#define RAM_MIRRORS_END 0x0FFF

#define PPU_REGISTERS 0x2000
#define PPU_REGISTERS_END 0x3FFF

static uint8_t memory[0xFFFF];
static uint8_t v_memory[0x800];

static vector<uint8_t> PRGrom;
static vector<uint8_t> CHRrom;
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
		printf("PC: %x  instruction: %x \n", (uint16_t)(i + 0x8000), instructions[i]);
		write_8bit((i + 0x8000), instructions[i]);
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
void load_rom(uint16_t chr_rom_start, uint16_t chr_size, uint16_t prg_rom_start, uint16_t prg_size, vector<uint8_t> instructions)
{
	// copy(instructions.begin(), instructions.end(), PRGrom.begin() + prg_size);
	for (int i = prg_rom_start; i < prg_size; i++)
	{
		PRGrom.push_back(instructions[i]);
	}
	for (int i = chr_rom_start; i < chr_size; i++)
	{
		CHRrom.push_back(instructions[i]);
	}
}