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
            struct
            {
                unsigned B : 1; // color emphasis Blue
                unsigned R : 1; // color emphasis Red
                unsigned G : 1; // color emphasis Green
                unsigned s : 1; // sprite enable
                unsigned b : 1; // background enable
                unsigned M : 1; // sprite left column disable
                unsigned m : 1; // background left column disable
                unsigned g : 1; // greyscale
            };
            uint8_t val;
        } ppumask;
        union
        {
            struct
            {
                uint8_t hi;
                uint8_t lo;
            };
            uint16_t val;
        } ppuAddr;
        union
        {
            struct
            {
                unsigned P : 1; // PPU master/slave
                unsigned V : 1; // NMI enable
                unsigned H : 1; // sprite height
                unsigned B : 1; // background tile select
                unsigned S : 1; // sprite tile select (ignored in 8x16 sprite mode)
                unsigned I : 1; // increment mode
                unsigned N : 2; // increment mode
            };
            uint8_t val;

        } ppuCtrl;
        union
        {
            struct
            {
                unsigned V : 1;
                unsigned S : 1;
                unsigned O : 1;
                unsigned unused1 : 1;
                unsigned unused2 : 1;
                unsigned unused3 : 1;
                unsigned unused4 : 1;
            };
            uint8_t val;

        } ppuStatus;
        bool scrollLatch;
    };
    uint8_t memory[0x800];
    Registers reg;
    std::vector<uint8_t> chr_rom;
    uint8_t oam[0x80];
    uint16_t oam_addr;
    uint8_t pallete[0x20];
    MirrorType mirrorType;
    uint8_t internalDataBuffer;
    uint16_t mirror(uint16_t address);
    size_t cycles;
    uint16_t scanline;

public:
    PPU(std::vector<uint8_t> chr_rom, MirrorType mirrorType);
    PPU();

    uint8_t read_PPU_data();

    void write_PPU_address(uint8_t val);
    void write_PPU_ctrl(uint8_t val);
    void write_PPU_mask(uint8_t val);
    void write_PPU_data(uint8_t val);
    void tick(uint8_t clock_cycles);
};

#endif