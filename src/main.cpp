#include <iostream>
#include <emulator/Computer.h>
#include <emulator/LoadRom.h>
#include <vector>
#include <bitset>
// #include "SetupSFML.h"
#include <cassert>

int main(int argc, char *argv[])
{
	if (argc < 1)
	{
		std::cout << "need NES v 1.0 ROM as input file" << std::endl;
		return EXIT_FAILURE;
	}
	CPU cpu = init(argv[1]);

	//  uint8_t joy_pad_byte1 = 0b11010000; // Initial value with bits 0, 1, 2, 3 set
    // uint8_t mask = 0b00100000;          // Mask to set the 4th bit

    // std::cout << "Initial byte: " << std::bitset<8>(joy_pad_byte1) << std::endl;

    // joy_pad_byte1 |= mask;              // Set the 4th bit

    // std::cout << "Resulting byte: " << std::bitset<8>(joy_pad_byte1) << std::endl;
	return cpu.error_code;
}