#include <bitset>

#include <QDebug>

#include <Emulator/ppu_components.h>

PPUControl::PPUControl()
{
    this->reset();
}

void PPUControl::reset()
{
    this->ppuCtrl.val = 0;
}
void PPUControl::log()
{
    std::bitset<8> ppu_ctrl(this->ppuCtrl.val);
    std::bitset<2> name_table_address(this->ppuCtrl.N);
    qInfo() << "===== PPU ctrl ===="
    << "\nNMI enable (0: off, 1: on): " << this->ppuCtrl.V
    << "\nPPU master/slave select (0: read backdrop from EXT pins; 1: output color on EXT pins): "
        << this->ppuCtrl.P
    << "\nsprite size (0: 8x8, 1: 8x16): " << this->ppuCtrl.H
    << "\nBackground patterntable (0: $0000; 1: $1000): " << this->ppuCtrl.B
    << "\nSprite patterntable (0: $0000; 1: $1000): " << this->ppuCtrl.S
    << "\nincrement (0: add 1 going across, 1: add 32 going down): " << this->ppuCtrl.I
    << "\nname table addreess: " << name_table_address.to_string()
    << "\nctrl: " << ppu_ctrl.to_string()
    << "\n";
}
void PPUControl::set_bit(int index, int toggle)
{
    if (toggle == 1)
    {
        uint8_t mask = 1 << index;
        this->ppuCtrl.val |= mask;
    }
    else
    {
        uint8_t mask = (1 << index);
        this->ppuCtrl.val &= (~mask);
    }
}

int PPUControl::get_bit(int index)
{
    return (this->ppuCtrl.val >> index) & 1;
}

uint8_t PPUControl::read_8bit()
{
    return this->ppuCtrl.val;
}

void PPUControl::write_8bit(uint8_t value)
{
    this->ppuCtrl.val = value;
}

//
