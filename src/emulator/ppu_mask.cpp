#include <bitset>

#include <QDebug>

#include <Emulator/ppu_components.h>

PPUMask::PPUMask()
{
    this->reset();
}

void PPUMask::log()
{
    std::bitset<8> ppu_mask(this->ppumask.val);
    qInfo() << "===== PPU mask ===="
    << "\nEmphasize blue: " << this->ppumask.B
    << "\nEmphasize green: " << this->ppumask.G
    << "\nEmphasize red: " << this->ppumask.R

    << "\nEnable sprite rendering: " << this->ppumask.s
    << "\nEnable background rendering: " << this->ppumask.b
    << "\nShow sprites in leftmost 8 pixels of screen: " << this->ppumask.M
    << "\nShow background in leftmost 8 pixels of screen " << this->ppumask.m
    << "\ngrey scale (0: normal color, 1: grey scale): " << this->ppumask.g
    << "\nppu mask: " << ppu_mask.to_string()
    << "\n";
}

void PPUMask::reset()
{
    this->ppumask.val = 0b00000000;
}

void PPUMask::write_8bit(uint8_t value)
{
    this->ppumask.val = value;
}
void PPUMask::set_bit(int index, int toggle)
{
    if (toggle == 1)
    {
        uint8_t mask = 1 << index;
        this->ppumask.val |= mask;
    }
    else
    {
        uint8_t mask = (1 << index);
        this->ppumask.val &= (~mask);
    }
}

int PPUMask::get_bit(int index)
{
    return (this->ppumask.val >> index) & 1;
}

//