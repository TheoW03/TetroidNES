#pragma once

#include <iostream>
#include <vector>
#include <bit>
#include <optional>
#include <chrono>
#include <cstdint>

// #include <SFML/Graphics.hpp>
#include <memory>
#include <Emulator/ppu_components.h>
#include <Emulator/LoadRom.h>

using renderdata = std::array<uint8_t, 245760ULL>;
using renderdata_shared_ptr = std::shared_ptr<renderdata>;

union ColorPalette
{
    uint8_t rgba[4];
    struct
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };
};

class PPU
{
private:
    ColorPalette bg_pallete(size_t row, size_t column);
    ColorPalette getColorFromByte(uint16_t byte, ColorPalette &pallete);
    void get_chr_tile(uint16_t tile_idx, int banks, std::vector<uint8_t> &tile_list);
    PPUMask ppumask;
    PPUAddress ppuaddr;
    PPUStatus ppustatus;
    PPUControl ppuctrl;
    struct Registers
    {
        bool scrollLatch;
    };
    std::array<uint8_t, 0x800> memory;
    Registers reg;
    std::vector<uint8_t> chr_rom;

    std::array<uint8_t, 256> oam;
    uint8_t oam_addr;
    std::array<uint8_t, 0x20> pallete;
    MirrorType mirrorType;
    uint8_t internalDataBuffer;
    uint16_t mirror(uint16_t address);
    size_t cycles;
    uint16_t scanline;
    renderdata_shared_ptr rgb_ds; // Unique ptr will implicitly delete copy operator for this class

    void draw_background(renderdata &rgb_ds, int banks);
    void draw_sprites(renderdata &rgb_ds, int banks);

public:
    PPU(std::vector<uint8_t> chrrom, MirrorType mirrorType);
    PPU();

    uint8_t read_PPU_data();
    uint8_t read_OAM_data();
    std::optional<std::string> err_string;
    std::chrono::high_resolution_clock::time_point start;
    void write_PPU_address(uint8_t val);
    void write_PPU_ctrl(uint8_t val);
    void write_PPU_mask(uint8_t val);
    std::optional<int> write_PPU_data(uint8_t val);
    void write_OAM_data(uint8_t val);
    void write_OAM_address(uint8_t val);
    bool NMI_interrupt(uint8_t clock_cycles);
    bool tick(uint8_t clock_cycles);
    uint8_t read_status();
    void print_ppu_stats();

    // void render(sf::Texture &texture, int bank, int tile);
    renderdata_shared_ptr render_texture();
    void log_ppu();
};