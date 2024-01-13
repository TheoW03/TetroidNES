#include <iostream>
#include "../src/emulator/BitOperations.h"
#include "../src/emulator/Memory.h"
#include "../src/emulator/Computer.h"
#include "../src/emulator/LoadRom.h"
#include <vector>
using namespace std;
int main(int argc, char *argv[])
{
	vector<uint8_t> instructions = load_rom("games/test.bin"); // random bin file
	// vector<uint8_t> instructions = {0xA9 ,0x0A, 0xC9 ,0x0A, 0xF0 ,0x02, 0xD0, 0x03,0xA9,0x14, 0x00 ,0xA9, 0x1E, 0x00};
	load_instructions(instructions);
	run();
}
