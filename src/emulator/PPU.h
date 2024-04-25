#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
// #include "Computer.h"
#include "LoadRom.h"
using namespace std;

#ifndef PPU_H
#define PPU_H
class PPU
{
private:
    uint8_t memory[0x800];
    vector<uint8_t> chr_rom;
    uint8_t oam[0x80];
    uint8_t pallete[0x20];
    MirrorType mirrorType;

public:
    PPU(vector<uint8_t> chr_rom, MirrorType mirrorType);
    PPU();

    uint8_t read_8bit_PPU(uint16_t address);
    void write_16bit_PPU(uint16_t address, uint16_t value);

    void write_8bit_PPU(uint16_t address, uint8_t value);

    uint16_t read_16bit_PPU(uint16_t address);
};

#endif