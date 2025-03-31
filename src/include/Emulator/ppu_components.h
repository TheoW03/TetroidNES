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

#ifndef PPU_MASK_H
#define PPU_MASK_H
class PPUMask : public ComponentRegister
{
public:
    PPUMask();
    void reset() override;
    void log() override;
    void set_bit(int index, int toggle) override;
    int get_bit(int index) override;
};
#endif

#ifndef PPU_Ctrl_H
#define PPU_Ctrl_H
class PPUControl : public ComponentRegister
{
private:
    union
    {
        struct
        {
            unsigned N : 2; // increment mode
            unsigned I : 1; // increment mode
            unsigned S : 1; // sprite tile select (ignored in 8x16 sprite mode)

            unsigned B : 1; // background tile select

            unsigned H : 1; // sprite height

            unsigned P : 1; // PPU master/slave

            unsigned V : 1; // NMI enable
        };
        uint8_t val;

    } ppuCtrl;

public:
    PPUControl();
    void reset() override;
    void log() override;
    void set_bit(int index, int toggle) override;
    int get_bit(int index) override;
    uint8_t read_8bit() override;
    void write_8bit(uint8_t value) override;
};
#endif