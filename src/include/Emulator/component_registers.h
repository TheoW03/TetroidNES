#include <stdint.h>

#ifndef COMPONENT_H
#define COMPONENT_H
class ComponentRegister
{
public:
    ComponentRegister();
    virtual void write_8bit(uint8_t value);
    virtual uint8_t read_8bit();
    virtual void write_16bit(uint16_t value);
    virtual uint16_t read_16bit();
    virtual void reset() = 0;
    virtual void log() = 0;
    virtual int get_bit(int inudex);
    virtual void set_bit(int index);
};
#endif

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