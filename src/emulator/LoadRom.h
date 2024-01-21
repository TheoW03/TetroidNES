#include <iostream>
#include <vector>

using namespace std;
struct Rom
{
    vector<uint8_t> instructions;
    uint16_t PRG_ROM;
    uint16_t CHR_ROM;
};

vector<uint8_t> load_rom(string file_name);
void modify_for_NESfile(vector<uint8_t> &instructions);