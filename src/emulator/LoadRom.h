#include <iostream>
#include <vector>

using namespace std;
vector<uint8_t> load_rom(string file_name);
void modify_for_NESfile(vector<uint8_t> &instructions);