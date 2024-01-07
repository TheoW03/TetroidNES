#include<iostream>
#include <vector>
using namespace std;





uint8_t read_8bit(uint16_t address);

void write_16bit(uint16_t address, uint16_t value);

void write_8bit(uint16_t address, uint8_t value);

uint16_t read_16bit(uint16_t address);

void load_instructions(vector<uint8_t> instructions);
