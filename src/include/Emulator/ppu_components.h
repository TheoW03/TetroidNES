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
    void reset_latch() override;
};
#endif

#ifndef PPU_Status_H
#define PPU_Status_H
class PPUStatus : public ComponentRegister
{
private:
    union
    {
        struct
        {
            unsigned padding : 5;

            unsigned O : 1;
            unsigned S : 1;
            unsigned V : 1;
        };
        uint8_t val;

    } ppuStatus;

public:
    PPUStatus();
    void write_8bit(uint8_t value) override;
    uint8_t read_8bit() override;
    void set_bit(int index, int toggle) override;
    int get_bit(int index) override;
    void log() override;
    void reset() override;
};
#endif