#include <iostream>
#include <vector>

using namespace std;
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
struct Rom
{
    vector<uint8_t> PRG;
    vector<uint8_t> CHR;
    uint8_t mapper;
    MirrorType mirror;
};
#endif

vector<uint8_t> file_tobyte_vector(string file_name);
Rom load_rom(vector<uint8_t> instructions);