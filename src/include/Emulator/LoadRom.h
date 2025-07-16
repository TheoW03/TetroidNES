#pragma once

#include <array>
#include <iostream>
#include <vector>
#include <optional>
#include <cstdint>

enum MirrorType
{
    VERTICAL,
    HORIZONTAL,
    FOUR_SCREEN,
};

enum ColorEncoding
{
    Pal,
    Ntsc
};

#define PRG_ROM_SIZE_16 16384
#define PRG_ROM_SIZE_32 32768
#define CHR_ROM_SIZE 8192

// easier to understand over doing bytes and bit operations
typedef uint8_t byte_t;
struct NESHeader
{
    byte_t ident[4]; // should be the words "NES 0x1a"
    byte_t prg_size; // size of PRG
    byte_t chr_size; // size of chr
    union
    {
        struct
        {
            unsigned mapper_lower : 4; // lower bits of mapper
            unsigned four_screen : 1;  // if 4 screen
            unsigned trainer : 1;      // trainer. Ie the PRG is a 512 offset
            unsigned battery : 1;      // for zelda (it used a battery to save )
            unsigned vertical : 1;     // if vertical
        };
        byte_t val;
    } flag6;

    union
    {
        struct
        {
            unsigned mapper_upper : 4; // upper bits of mapper
            unsigned inesverif : 4;    // 0xc to verify if its ines 1.0 format
        };
        byte_t val;
    } flag7;

    byte_t flag8;

    union
    {
        struct
        {
            unsigned color_encoding : 1; // NTSC vs pal
            unsigned padding : 7;
        };
        byte_t val;
    } flag9;
};

struct Rom
{
    std::vector<uint8_t> PRG;
    std::vector<uint8_t> CHR;
    uint8_t mapper;
    MirrorType mirror;
    ColorEncoding color_encoding;

    size_t prg_size;
    size_t chr_size;
};

std::vector<uint8_t> file_tobyte_vector(std::string file_name);
std::optional<Rom> load_rom(const std::vector<uint8_t> &instructions);