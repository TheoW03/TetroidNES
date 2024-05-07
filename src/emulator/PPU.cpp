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
    else if (addr == 0x3f10 || addr == 0x3f14 || addr == 0x3f18 || addr == 0x3f1c)
    {
        addr = addr - 0x10;
        return pallete[addr - 0x3f00];
    }
    return 0;
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
    }
    else if (addr == 0x3f10 || addr == 0x3f14 || addr == 0x3f18 || addr == 0x3f1c)
    {
        addr = addr - 0x10;
        pallete[addr - 0x3f00] = val;
    }
}

void PPU::tick(uint8_t clock_cycles)
{
    this->cycles += clock_cycles;
}
void PPU::render(sf::Texture texture)
{
    std::vector<uint8_t> tile;
    int bank  = reg.ppuCtrl.B;
    // int bank =
    // for (int i = bank + tiles * 16; i < bank + tiles * 16 + 15; i++)
    // {
    // tile.push_back(chr_rom[i]);
    // }
}

uint8_t PPU::read_OAM_data()
{

    return oam[oam_addr];
}
void PPU::write_OAM_data(uint8_t val)
{
    oam[oam_addr] = val;
    oam_addr++;
}
