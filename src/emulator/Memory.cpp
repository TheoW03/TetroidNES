#include <iostream>
#include <vector>

using namespace std;


static uint8_t memory[0xFFF];


uint8_t read_8bit(uint16_t address){
	return memory[address];
}

uint16_t read_16bit(uint16_t address){
	uint16_t value =(uint16_t)(memory[address] << 8) | memory[address+1];
	return value;
}
void write_8bit(uint16_t address, uint8_t value){
	memory[address] = value;
}
void write_16bit(uint16_t address, uint16_t value){
 	uint8_t msb = (uint8_t)(value >> 8);
    	uint8_t lsb = (uint8_t)(value & 0xFF);
	memory[address] = msb;
	memory[address + 1] = lsb;
}

void load_instructions(vector<uint8_t> instructions){
	for(int i = 0; i < instructions.size();i++){
		write_8bit(i, instructions[i]);
	}
}


