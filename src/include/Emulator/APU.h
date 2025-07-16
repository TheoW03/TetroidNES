#pragma once

#include <array>
#include <iostream>
#include <cstdint>

class APU
{
private:
    std::array<uint8_t, 0x17> apu_ram;

public:
    APU();
};