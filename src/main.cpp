#include <iostream>
#include "../src/emulator/BitOperations.h"
#include "../src/emulator/Memory.h"
#include "../src/emulator/Computer.h"
#include "SetupSDL.h"
#include "../src/emulator/LoadRom.h"
#include <vector>
#include <bitset>
#include <cassert>
using namespace std;
int main(int argc, char *argv[])
{
	// char* name = "test";
	// setup(name, 800, 600);
	// vector<uint8_t> instructions = load_rom("games/test4.bin"); // random bin file
	// vector<uint8_t> instructions = {0xF8, 0xa9, 0x10, 0xE9, 0x05};
	// load_instructions(instructions);
	// write_16bit(0xFFFC, 0x8000);
	// assert((-1<< 7) != 0) == 1);
	// assert(1 == 1);
	// assert(((uint8_t)(-1 << 7) != 0) == 1);
	// assert(((uint8_t)(2 << 7) != 0) == 0);
	// assert((uint8_t)(3 & 1) == 0);
	// assert((uint8_t)(2 & 1) == 0);
	// assert(((uint8_t)(127 & (0b00100000)) != 0) == 1);
	// assert(((uint8_t)(128 & (0b00100000)) != 0) == 0);


	// assert(((uint8_t)(2 << 7)) != 0);
	run();
	return 1;
}
// bool checkCarryFlag(uint8_t result)
// {
// 	// Check carry from bit 7 (overflow) or underflow from bit 0
// 	return (result & 0x80) || (result == 0);
// }

// int main()
// {
// 	// Example: 255 + 1 (overflow)
// 	uint8_t num1 = 255;
// 	uint8_t num2 = 2;
// 	uint8_t result = num1 + num2;

// 	// Check carry flag
// 	bool carryFlag = checkCarryFlag(result);

// 	if (carryFlag)
// 	{
// 		std::cout << "Carry flag set (overflow or underflow)!" << std::endl;
// 		std::cout << "Result: " << std::bitset<8>(result) << std::endl;
// 	}
// 	else
// 	{
// 		std::cout << "Result: " << std::bitset<8>(result) << std::endl;
// 	}

// 	return 0;
// }