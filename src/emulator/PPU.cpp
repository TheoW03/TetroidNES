#include "PPU.h"
#include <SFML/Graphics.hpp>

PPU::PPU(std::vector<uint8_t> chr_rom, MirrorType mirrorType)
{
    this->chr_rom = chr_rom;
    this->mirrorType = mirrorType;
    this->internalDataBuffer = 0;
    this->reg.ppuAddr.val = 0;
    this->reg.ppuCtrl.val = 0;
    this->reg.ppumask.val = 0;
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

uint8_t PPU::read_PPU_data()
{
    uint16_t addr = this->reg.ppuAddr.hi << 8 | (this->reg.ppuAddr.lo & 0xffff);
    this->reg.ppuAddr.val += reg.ppuCtrl.I ? 32 : 1;
    if (addr >= 0 && addr <= 0x1fff)
    {
        uint8_t res = internalDataBuffer;
        internalDataBuffer = chr_rom[addr];
        return res;
    }
    else if (addr >= 0x2000 && addr <= 0x2fff)
    {
        uint8_t res = internalDataBuffer;
        internalDataBuffer = memory[mirror(addr)];
        return res;
    }
    return 0;
    // return 0;
}
void PPU::write_PPU_address(uint8_t val)
{
    if (this->reg.scrollLatch)
    {
        this->reg.ppuAddr.lo = val;
    }
    else
    {
        this->reg.ppuAddr.hi = val;
    }
    this->reg.scrollLatch = !this->reg.scrollLatch;
}
void PPU::write_PPU_ctrl(uint8_t val)
{
    this->reg.ppuCtrl.val = val;
}
void PPU::write_PPU_mask(uint8_t val)
{
    this->reg.ppumask.val = val;
}
void PPU::write_PPU_data(uint8_t val)
{
    uint16_t addr = this->reg.ppuAddr.hi << 8 | (this->reg.ppuAddr.lo & 0xffff);
    this->reg.ppuAddr.val += reg.ppuCtrl.I ? 32 : 1;
    if (addr >= 0x2000 && addr <= 0x2fff)
    {
        uint8_t res = internalDataBuffer;
        internalDataBuffer = memory[mirror(addr)];
        return res;
    }
}

void PPU::tick(uint8_t cycles)
{
    this->cycles += cycles;
}
