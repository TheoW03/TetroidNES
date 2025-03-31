#include <Emulator/ppu_components.h>
#include <QDebug>
#include <bitset>
PPUMask::PPUMask()
{
    this->reset();
}

void PPUMask::log()
{
    std::bitset<8> ppu_mask(this->ppumask.val);
    qInfo() << "Emphasize blue: " << this->ppumask.B;
    qInfo() << "Emphasize green: " << this->ppumask.G;
    qInfo() << "Emphasize red: " << this->ppumask.R;

    qInfo() << "Enable sprite rendering: " << this->ppumask.s;
    qInfo() << "Enable background rendering: " << this->ppumask.b;
    qInfo() << "Show sprites in leftmost 8 pixels of screen: " << this->ppumask.M;
    qInfo() << "Show background in leftmost 8 pixels of screen " << this->ppumask.m;
    qInfo() << "grey scale (0: normal color, 1: grey scale): " << this->ppumask.g;
    qInfo() << "ppu mask: " << ppu_mask.to_string();
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