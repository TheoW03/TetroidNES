#include <stdint.h>

/*

for the NES's many memory mapped devices. this will be the class

that we will have each one
extend such as PPU addr, PPU ctrl, PPU status...

*/
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
    virtual void reset_latch();
    virtual void log() = 0;
    virtual int get_bit(int index);

    virtual void set_bit(int index, int toggle);
};
#endif
