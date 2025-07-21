#include <QDebug>

#include <Qt/utils/util.h>
#include <Emulator/ppu_components.h>

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
    qInfo() << "=====PPU ADDR====="
    << "\nPPU addr:  " << num_to_hexa(this->ppuAddr.val)
    << "\nlo:  " << num_to_hexa(this->ppuAddr.lo)
    << "\nhi:  " << num_to_hexa(this->ppuAddr.hi);
}

void PPUAddress::increment(int toggle)
{
    if (toggle == 1)
    {
        this->ppuAddr.val += 32;
    }
    else
    {
        this->ppuAddr.val += 1;
    }
}
void PPUAddress::reset_latch()
{
    this->high_ptr = true;
}
