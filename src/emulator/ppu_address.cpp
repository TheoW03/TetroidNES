#include <Emulator/component_registers.h>

PPUAddress::PPUAddress()
{
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
}
