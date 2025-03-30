#include <Emulator/ppu_components.h>
#include <Qt/util.h>
#include <QDebug>

PPUAddress::PPUAddress()
{
    this->reset();
}

void PPUAddress::write_8bit(uint8_t value)
{
    if (this->high_ptr)
    {
        this->ppuAddr.hi = value;
    }
    else
    {
        this->ppuAddr.lo = value;
    }
    this->high_ptr = !high_ptr;
}

void PPUAddress::write_16bit(uint16_t value)
{
    this->ppuAddr.val = value;
}
uint16_t PPUAddress::read_16bit()
{
    return this->ppuAddr.val;
}

void PPUAddress::reset()
{
    this->ppuAddr.val = 0;
    this->high_ptr = true;
}

void PPUAddress::log()
{
    qInfo() << "PPU addr:  " << num_to_hexa(this->ppuAddr.val);
    qInfo() << "lo:  " << num_to_hexa(this->ppuAddr.lo);
    qInfo() << "hi:  " << num_to_hexa(this->ppuAddr.hi);
}

void PPUAddress::reset_latch()
{
    this->high_ptr = true;
}
