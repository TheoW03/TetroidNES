#include <Emulator/ppu_components.h>
#include <QDebug>
#include <bitset>
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
    qInfo() << "NMI enable (0: off, 1: on): " << this->ppuCtrl.V;
    qInfo() << "PPU master/slave select (0: read backdrop from EXT pins; 1: output color on EXT pins): "
            << this->ppuCtrl.P;
    qInfo() << "sprite size (0: 8x8, 1: 8x16): " << this->ppuCtrl.H;
    qInfo() << "Background patterntable (0: $0000; 1: $1000): " << this->ppuCtrl.B;
    qInfo() << "Sprite patterntable (0: $0000; 1: $1000): " << this->ppuCtrl.S;
    qInfo() << "increment (0: add 1 going across, 1: add 32 going down): " << this->ppuCtrl.I;
    std::bitset<2> name_table_address(this->ppuCtrl.N);
    qInfo() << "name table addreess: " << name_table_address.to_string();
    qInfo() << "ctrl: " << ppu_ctrl.to_string();
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
