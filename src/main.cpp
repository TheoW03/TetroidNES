#include <iostream>
#include "../src/emulator/BitOperations.h"
#include "../src/emulator/Memory.h"
#include "../src/emulator/Computer.h"
#include <vector>
using namespace std;
int main(int argc, char *argv[])
{
	// vector<uint8_t> instructions = {0xa9, 129, 0x00};
	// load_instructions(instructions);
	cout << (int)add(254, 2) << endl;
	run();
}
