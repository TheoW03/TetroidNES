#pragma once

#include <iostream>
#include <vector>
#include <bit>
#include <optional>
#include <tuple>
#include <chrono>
#include <cstdint>

// #include <SFML/Graphics.hpp>

#include <Emulator/ppu_components.h>
#include <Emulator/LoadRom.h>

union ColorPalette {
    uint8_t rgba[4];
    struct {
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
    std::shared_ptr<std::vector<uint8_t>> rgb_ds; // Unique ptr will implicitly delete copy operator for this class

    void draw_background(std::vector<uint8_t> &rgb_ds, int banks);
    void draw_sprites(std::vector<uint8_t> &rgb_ds, int banks);

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
    std::vector<uint8_t>* render_texture();
    void log_ppu();
};