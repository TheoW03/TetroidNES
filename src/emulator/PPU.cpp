#include "PPU.h"
#include <SFML/Graphics.hpp>

PPU::PPU(std::vector<uint8_t> chr_rom, MirrorType mirrorType)
{
    this->chr_rom = chr_rom;
    this->mirrorType = mirrorType;
    this->internalDataBuffer = 0;
}

PPU::PPU()
{
}
uint16_t PPU::mirror(uint16_t address)
{
    uint16_t mirrored_vram = address & 0b10111111111111;
    uint16_t m = mirrored_vram - 0x2000;
    uint16_t nameTable = m / 0x400;
    if ((mirrorType == MirrorType::VERTICAL) && (nameTable == 2 || nameTable == 3))
        m -= 0x800;
    else if ((mirrorType == MirrorType::HORIZONTAL) && (nameTable == 1 || nameTable == 2))
        m -= 0x400;
    else if (mirrorType == MirrorType::HORIZONTAL && nameTable == 3)
        m -= 0x800;
    return m;
}

uint8_t PPU::read_PPU(uint16_t address)
{
    if (registers.addr >= 0 && registers.addr <= 0x1fff)
    {
        uint8_t res = internalDataBuffer;
        internalDataBuffer = chr_rom[registers.addr];
        return res;
    }
    else if (registers.addr >= 0x2000 && registers.addr <= 0x2fff)
    {
        uint8_t res = internalDataBuffer;
        internalDataBuffer = memory[mirror(registers.addr)];
        return res;
    }
    return 0;
}
void PPU::write_PPU(uint16_t address, uint8_t value)
{
}
