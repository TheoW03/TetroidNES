#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
// #include "Computer.h"
#include "LoadRom.h"

#ifndef PPU_REG
#define PPU_REG
struct ppu_registers
{
    uint16_t addr;
    uint8_t ctrl;
    uint8_t mask;
    uint8_t OAM_data;
    uint8_t OAM_addr;
    uint8_t scroll;
    uint8_t status;
};
#endif

#ifndef PPU_H
#define PPU_H
class PPU
{
private:
    uint8_t memory[0x800];
    ppu_registers reg;
    std::vector<uint8_t> chr_rom;
    uint8_t oam[0x80];
    uint8_t pallete[0x20];
    MirrorType mirrorType;
    ppu_registers registers;
    uint8_t internalDataBuffer;

public:
    PPU(std::vector<uint8_t> chr_rom, MirrorType mirrorType);
    PPU();

    uint8_t read_PPU(uint16_t address);
    // void write_16bit_PPU(uint16_t address, uint16_t value);
    void write_PPU(uint16_t address, uint8_t value);
    uint16_t mirror(uint16_t address);

    // uint16_t read_16bit_PPU(uint16_t address);
};

#endif