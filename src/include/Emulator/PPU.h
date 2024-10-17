#include <iostream>
#include <vector>
// #include <SFML/Graphics.hpp>
#include <bit>
#include "LoadRom.h"
#include <optional>
#include <tuple>

#ifndef PPU_H
#define PPU_H
#include <cstdint>
class PPU
{
private:
    std::tuple<uint8_t, uint8_t, uint8_t> getColorFromByte(uint16_t byte);
    // sf::Color getColorFromByte(uint16_t byte);
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
                unsigned lo : 8;
                unsigned hi : 8;
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
                unsigned padding : 4;

                unsigned O : 1;
                unsigned S : 1;

                unsigned V : 1;
            };
            uint8_t val;

        } ppuStatus;
        bool scrollLatch;
        bool high_ptr;
    };
    uint8_t memory[0x800];
    Registers reg;
    std::vector<uint8_t> chr_rom;
    uint8_t oam[256];
    uint8_t oam_addr;
    uint8_t pallete[0x20];
    MirrorType mirrorType;
    uint8_t internalDataBuffer;
    uint16_t mirror(uint16_t address);
    size_t cycles;
    uint16_t scanline;

public:
    PPU(std::vector<uint8_t> chrrom, MirrorType mirrorType);
    PPU();

    uint8_t read_PPU_data();
    uint8_t read_OAM_data();

    void write_PPU_address(uint8_t val);
    void write_PPU_ctrl(uint8_t val);
    void write_PPU_mask(uint8_t val);
    std::optional<int> write_PPU_data(uint8_t val);
    void write_OAM_data(uint8_t val);
    void write_OAM_dma(uint8_t val[256]);
    void write_OAM_address(uint8_t val);
    bool NMI_interrupt(uint8_t clock_cycles);
    bool tick(uint8_t clock_cycles);
    uint8_t read_status();
    void print_ppu_stats();

    // void render(sf::Texture &texture, int bank, int tile);
    std::vector<uint8_t> render_texture(std::tuple<size_t, size_t> res);
};

#endif