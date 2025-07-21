#pragma once

#include <iostream>
#include <cstdint>

class APU
{
private:
    uint8_t apu_ram[0x17];

public:
    APU();
};