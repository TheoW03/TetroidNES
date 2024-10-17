#include <iostream>
#include <vector>

#ifndef MIRROR_TYPE_H
#define MIRROR_TYPE_H
enum MirrorType
{
    VERTICAL,
    HORIZONTAL,
    FOUR_SCREEN,
};
#endif
#ifndef ROM_H
#define ROM_H
#include <cstdint>
struct Rom
{
    std::vector<uint8_t> PRG;
    std::vector<uint8_t> CHR;
    uint8_t mapper;
    MirrorType mirror;
};
#endif

std::vector<uint8_t> file_tobyte_vector(std::string file_name);
Rom load_rom(std::vector<uint8_t> instructions);