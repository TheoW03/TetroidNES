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
	if (argc > 1)
		init(argv[1]);
	else
		std::cout << "need NES v 1.0 ROM as input file" << std::endl;
	return 1;
}