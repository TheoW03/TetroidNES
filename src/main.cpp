#include <iostream>
#include "../src/emulator/BitOperations.h"
#include "../src/emulator/Memory.h"
#include "../src/emulator/Computer.h"
#include <vector>
using namespace std;
int main(int argc, char *argv[])
{
	vector<uint8_t> myVector = {0xa9, 0x01, 0x00};
	load_instructions(myVector);
	run();
	
}
