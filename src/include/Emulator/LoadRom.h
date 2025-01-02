#pragma once

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

struct Rom
{
    std::vector<uint8_t> PRG;
    std::vector<uint8_t> CHR;
    uint8_t mapper;
    MirrorType mirror;
    ColorEncoding color_encoding;
};

std::vector<uint8_t> file_tobyte_vector(std::string file_name);
std::optional<Rom> load_rom(std::vector<uint8_t> instructions);