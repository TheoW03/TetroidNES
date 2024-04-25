#include "PPU.h"
#include <SFML/Graphics.hpp>

PPU::PPU(vector<uint8_t> chr_rom, MirrorType mirrorType)
{
    this->chr_rom = chr_rom;
    this->mirrorType = mirrorType;
}

PPU::PPU()
{
}
uint8_t PPU::read_8bit_PPU(uint16_t address)
{
    return 0;
}
uint16_t PPU::read_16bit_PPU(uint16_t addresss)
{
    return 0;
}
void PPU::write_16bit_PPU(uint16_t address, uint16_t value)
{
}
void PPU::write_8bit_PPU(uint16_t address, uint8_t value)
{
}