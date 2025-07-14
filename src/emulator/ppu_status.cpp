#include <bitset>

#include <QDebug>

#include <Emulator/ppu_components.h>

PPUStatus::PPUStatus()
{
    this->reset();
}

void PPUStatus::log()
{

    std::bitset<8> ppu_status(ppuStatus.val);
    qInfo() << "===== PPU status ===="
    << "\nVBlank: " << this->ppuStatus.V
    << "\n0_hit: " << this->ppuStatus.S
    << "\noverflow: " << this->ppuStatus.O
    << "\nstatus: " << ppu_status.to_string()
    << "\n";
}

void PPUStatus::reset()
{
    this->ppuStatus.val = 0;
}

void PPUStatus::write_8bit(uint8_t value)
{
    this->ppuStatus.val = value;
}

uint8_t PPUStatus::read_8bit()
{
    return this->ppuStatus.val;
}

void PPUStatus::set_bit(int index, int toggle)
{

    if (toggle == 1)
    {
        uint8_t mask = 1 << index;
        this->ppuStatus.val |= mask;
    }
    else
    {
        uint8_t mask = (1 << index);
        this->ppuStatus.val &= (~mask);
    }
}

int PPUStatus::get_bit(int index)
{
    qInfo() << "get bit of status: " << std::to_string((this->ppuStatus.val >> index) & 1);
    return (this->ppuStatus.val >> index) & 1;
}