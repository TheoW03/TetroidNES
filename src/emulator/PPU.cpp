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
sf::Color PPU::getColorFromByte(uint8_t byte)
{
    sf::Color system_palette[64] = {
        sf::Color(0x80, 0x80, 0x80), sf::Color(0x00, 0x3D, 0xA6), sf::Color(0x00, 0x12, 0xB0), sf::Color(0x44, 0x00, 0x96),
        sf::Color(0xA1, 0x00, 0x5E), sf::Color(0xC7, 0x00, 0x28), sf::Color(0xBA, 0x06, 0x00), sf::Color(0x8C, 0x17, 0x00),
        sf::Color(0x5C, 0x2F, 0x00), sf::Color(0x10, 0x45, 0x00), sf::Color(0x05, 0x4A, 0x00), sf::Color(0x00, 0x47, 0x2E),
        sf::Color(0x00, 0x41, 0x66), sf::Color(0x00, 0x00, 0x00), sf::Color(0x05, 0x05, 0x05), sf::Color(0x05, 0x05, 0x05),
        sf::Color(0xC7, 0xC7, 0xC7), sf::Color(0x00, 0x77, 0xFF), sf::Color(0x21, 0x55, 0xFF), sf::Color(0x82, 0x37, 0xFA),
        sf::Color(0xEB, 0x2F, 0xB5), sf::Color(0xFF, 0x29, 0x50), sf::Color(0xFF, 0x22, 0x00), sf::Color(0xD6, 0x32, 0x00),
        sf::Color(0xC4, 0x62, 0x00), sf::Color(0x35, 0x80, 0x00), sf::Color(0x05, 0x8F, 0x00), sf::Color(0x00, 0x8A, 0x55),
        sf::Color(0x00, 0x99, 0xCC), sf::Color(0x21, 0x21, 0x21), sf::Color(0x09, 0x09, 0x09), sf::Color(0x09, 0x09, 0x09),
        sf::Color(0xFF, 0xFF, 0xFF), sf::Color(0x0F, 0xD7, 0xFF), sf::Color(0x69, 0xA2, 0xFF), sf::Color(0xD4, 0x80, 0xFF),
        sf::Color(0xFF, 0x45, 0xF3), sf::Color(0xFF, 0x61, 0x8B), sf::Color(0xFF, 0x88, 0x33), sf::Color(0xFF, 0x9C, 0x12),
        sf::Color(0xFA, 0xBC, 0x20), sf::Color(0x9F, 0xE3, 0x0E), sf::Color(0x2B, 0xF0, 0x35), sf::Color(0x0C, 0xF0, 0xA4),
        sf::Color(0x05, 0xFB, 0xFF), sf::Color(0x5E, 0x5E, 0x5E), sf::Color(0x0D, 0x0D, 0x0D), sf::Color(0x0D, 0x0D, 0x0D),
        sf::Color(0xFF, 0xFF, 0xFF), sf::Color(0xA6, 0xFC, 0xFF), sf::Color(0xB3, 0xEC, 0xFF), sf::Color(0xDA, 0xAB, 0xEB),
        sf::Color(0xFF, 0xA8, 0xF9), sf::Color(0xFF, 0xAB, 0xB3), sf::Color(0xFF, 0xD2, 0xB0), sf::Color(0xFF, 0xEF, 0xA6),
        sf::Color(0xFF, 0xF7, 0x9C), sf::Color(0xD7, 0xE8, 0x95), sf::Color(0xA6, 0xED, 0xAF), sf::Color(0xA2, 0xF2, 0xDA),
        sf::Color(0x99, 0xFF, 0xFC), sf::Color(0xDD, 0xDD, 0xDD), sf::Color(0x11, 0x11, 0x11), sf::Color(0x11, 0x11, 0x11)};
    return system_palette[byte];
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
uint8_t PPU::read_status()
{
    return reg.ppuStatus.val;
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

bool PPU::tick(uint8_t clock_cycles)
{
    this->cycles += clock_cycles;
    if (this->cycles >= 341)
    {
        this->scanline += 1;
        this->cycles -= 341;
    }
    if (scanline >= 261)
    {

        scanline = 0;
        reg.ppuStatus.V = 0;
        return true;
    }

    return false;
}

bool PPU::NMI_interrupt(uint8_t clock_cycles)
{

    if (this->scanline == 241)
    {

        if (reg.ppuCtrl.V == 0)
        {
            reg.ppuStatus.V = 1;
            return true;
        }
    }
    return false;
}
void PPU::render(sf::Texture texture)
{
    uint8_t data[256 * 240 * 3];
    std::vector<uint8_t> tile;
    for (int i = 0; i < 0x3c; i++)
        int bank = reg.ppuCtrl.B ? 0 : 0x1000;
    for (int i = 0; i < 15; i++)
    {
        tile.push_back(chr_rom[i]);
    }
    texture.update(data);
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
