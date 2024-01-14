#include <iostream>
#include "../src/emulator/BitOperations.h"
#include "../src/emulator/Memory.h"
#include "../src/emulator/Computer.h"
#include "../src/emulator/LoadRom.h"
#include <vector>
using namespace std;
int main(int argc, char *argv[])
{
	vector<uint8_t> instructions = load_rom("games/test3.bin"); // random bin file
	// vector<uint8_t> instructions = {0x4C, 0x00, 0xC};
	load_instructions(instructions);
	run();
}
