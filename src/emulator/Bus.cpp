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
    if (address < 0x0FFF)
    {
        uint16_t mirroraddr = address & 0x7ff;
        return v_memory[mirroraddr];
    }
}

void Bus::write_8bit(uint16_t address, uint8_t value)
{
    if (address < 0x0FFF)
    {
        uint16_t mirroraddr = address & 0x7ff;
        v_memory[mirroraddr] = value;
    }
    // memory[address] = value;
}

uint16_t Bus::read_16bit(uint16_t address)
{
    if (address < 0x0FFF)
    {
        uint16_t mirroraddr = address & 0x7ff;
        uint16_t value = (uint16_t)(v_memory[mirroraddr + 1] << 8) | v_memory[mirroraddr];
        return value;
    }
}

void Bus::write_16bit(uint16_t address, uint16_t value)
{
    if (address < 0x0FFF)
    {
        uint16_t mirroraddr = address & 0x7ff;
        uint8_t msb = (uint8_t)(value >> 8);
        uint8_t lsb = (uint8_t)(value & 0xFF);
        v_memory[mirroraddr] = lsb;
        v_memory[mirroraddr + 1] = msb;
    }
}
