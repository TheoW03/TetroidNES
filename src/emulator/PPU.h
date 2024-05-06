#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <bit>
#include "LoadRom.h"

#ifndef PPU_H
#define PPU_H
class PPU
{
private:
    struct Registers
    {
        union
        {
            uint8_t raw;
            uint8_t B : 1; // color emphasis Blue
            uint8_t R : 1; // color emphasis Red
            uint8_t G : 1; // color emphasis Green
            uint8_t s : 1; // sprite enable
            uint8_t b : 1; // background enable
            uint8_t M : 1; // sprite left column disable
            uint8_t m : 1; // background left column disable
            uint8_t g : 1; // greyscale
        } ppumask;
        union
        {
            uint8_t hi;
            uint8_t lo;
        } ppuAddr;
        union
        {
            uint8_t P : 1; // PPU master/slave
            uint8_t V : 1; // NMI enable
            uint8_t H : 1; // sprite height
            uint8_t B : 1; // background tile select
            uint8_t S : 1; // sprite tile select (ignored in 8x16 sprite mode)
            uint8_t I : 1; // increment mode
            uint8_t N : 2; // increment mode

        } ppuCtrl;
    };
    uint8_t memory[0x800];
    Registers reg;
    std::vector<uint8_t> chr_rom;
    uint8_t oam[0x80];
    uint16_t oam_addr;
    uint8_t pallete[0x20];
    MirrorType mirrorType;
    uint8_t internalDataBuffer;

public:
    PPU(std::vector<uint8_t> chr_rom, MirrorType mirrorType);
    PPU();

    uint8_t read_PPU_data();
    // void write_16bit_PPU(uint16_t address, uint16_t value);
    void write_PPU(uint16_t address, uint8_t value);
    uint16_t mirror(uint16_t address);

    void inc_address(uint8_t value);
    // uint16_t read_16bit_PPU(uint16_t address);
};

#endif