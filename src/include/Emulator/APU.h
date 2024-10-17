#include <iostream>

#ifndef APU_H
#define APU_H
#include <cstdint>
class APU
{
private:
    uint8_t apu_ram[0x17];

public:
    APU();
};

#endif