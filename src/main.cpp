#include <iostream>
#include "../src/emulator/BitOperations.h"
#include "../src/emulator/Memory.h"
#include "../src/emulator/Computer.h"
#include "SetupSDL.h"
#include "../src/emulator/LoadRom.h"
#include <vector>
#include <bitset>
#include <pthread.h>
#include <cassert>
using namespace std;
void *threadFunction(void *arg)
{
	cout << "Thread i" << endl;
	// while (1)
	// 	;
	vector<uint8_t> instructions = {0xa5, 0xff, 0x4C, 0x00, 0x80};
	load_instructions(instructions);
	write_16bit(0xFFFC, 0x8000);
	run();
	// sleep(2);
	// pthread_exit(NULL);
}
int main(int argc, char *argv[])
{
	// vector<uint8_t> instructions = load_rom("games/test4.bin"); // random bin file
	pthread_t myThread;
	int result = pthread_create(&myThread, NULL, threadFunction, NULL);
	pthread_detach(myThread);
	// vector<uint8_t> instructions = {0xa9, 0x01, 0x69, 0x01, 0x4C, 0x00, 0x80};
	// load_instructions(instructions);
	// run();
	setup("test", 800, 600);

	// pthread_join()

	// pthread_join(myThread, NULL);

	// cout << "test | gay" << endl;
	return 1;
}