#include "Bus.h"
#include "PPU.h"
using namespace std;

const static uint8_t ram_end = 0x0FFF;
const static uint8_t ppu_end = 0x2000;

Bus::Bus()
{
}
Bus::Bus(Rom rom)
{
    this->rom = rom;
    PPU ppu(rom.CHR);
    this->ppu = ppu;
}
uint8_t Bus::read_8bit(uint16_t address)
{
    return memory[address];
}

void Bus::write_8bit(uint16_t address, uint8_t value)
{
    memory[address] = value;
}

uint16_t Bus::read_16bit(uint16_t address)
{
    uint16_t value = (uint16_t)(memory[address + 1] << 8) | memory[address];
    return value;
}

void Bus::write_16bit(uint16_t address, uint16_t value)
{
    uint8_t msb = (uint8_t)(value >> 8);
    uint8_t lsb = (uint8_t)(value & 0xFF);
    memory[address] = lsb;
    memory[address + 1] = msb;
}
