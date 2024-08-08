#include <iostream>
#include "../src/emulator/BitOperations.h"
#include "../src/emulator/Computer.h"
#include "../src/emulator/LoadRom.h"
#include <vector>
#include <bitset>
#include "SetupSFML.h"
#include <cassert>

int main(int argc, char *argv[])
{
	if (argc < 1)
	{
		std::cout << "need NES v 1.0 ROM as input file" << std::endl;
		return EXIT_FAILURE;
	}
	CPU cpu = init(argv[1]);
	return cpu.error_code;
}