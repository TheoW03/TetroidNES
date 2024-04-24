#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
using namespace std;

#ifndef PPU_H
#define PPU_H
class PPU
{
private:
    uint8_t memory[0x800];
    vector<uint8_t> chr_rom;
    uint8_t oam[0x80];

public:
    PPU(vector<uint8_t> chr_rom);
    PPU();
};

#endif