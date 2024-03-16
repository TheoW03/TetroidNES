#include <iostream>
#include <vector>

using namespace std;
#ifndef ROM_H
#define ROM_H
struct Rom
{
    vector<uint8_t> PRG;
    vector<uint8_t> CHR;
    uint8_t mapper;
};
#endif

vector<uint8_t> load_rom(string file_name);
Rom modify_for_NESfile(vector<uint8_t> instructions);