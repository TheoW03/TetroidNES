#include <Emulator/component_registers.h>
#ifndef PPU_Address_H
#define PPU_Address_H
class PPUAddress : public ComponentRegister
{
private:
    bool high_ptr;
    union
    {
        struct
        {
            unsigned lo : 8;
            unsigned hi : 8;
        };
        uint16_t val;
    } ppuAddr;

public:
    PPUAddress();
    void write_8bit(uint8_t value) override;
    void write_16bit(uint16_t value) override;
    uint16_t read_16bit() override;
    void reset() override;
    void log() override;
};
#endif