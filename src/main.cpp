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
	setup("test", 800, 600);
	// vector<uint8_t> instructions = load_rom("games/test4.bin"); // random bin file
	// vector<uint8_t> instructions = {0xF8, 0xa9, 0x10, 0xE9, 0x05};
	// load_instructions(instructions);
	// run();
	return 1;
}