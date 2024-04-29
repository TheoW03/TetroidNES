#include <iostream>
#include <vector>
#define PPU_V 0b1000000
#define PPU_S 0b0100000
#define PPU_O 0b0010000

uint8_t check_PPU_V(uint8_t PPUStatus)
{
    return PPUStatus & PPU_V;
}

uint8_t check_PPU_S(uint8_t PPUStatus)
{
    return PPUStatus & PPU_S;
}

uint8_t check_PPU_O(uint8_t PPUStatus)
{
    return PPUStatus & PPU_O;
}
uint8_t set_PPU_V(uint8_t PPUStatus, uint8_t value)
{
    return 0;
}

uint8_t set_PPU_S(uint8_t PPUStatus, uint8_t value)
{
    return 0;
}

uint8_t set_PPU_O(uint8_t PPUStatus, uint8_t value)
{

    return 0;
}
