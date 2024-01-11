#include <iostream>
#include "../src/emulator/BitOperations.h"
#include "../src/emulator/Memory.h"
#include "../src/emulator/Computer.h"
#include "../src/emulator/LoadRom.h"
#include <vector>
using namespace std;
int main(int argc, char *argv[])
{
	load_rom("games/Super Mario Bros (E).nes"); //random bin file
	//vector<uint8_t> instructions = {0xa9, 129, 0xE9, 0x3, 0x00};
	//load_instructions(instructions);
	
//	run();
}
